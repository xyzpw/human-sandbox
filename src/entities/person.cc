#include "entities/person.hpp"
#include <raylib.h>
#include <raymath.h>
#include <box2d/box2d.h>
#include <string>
#include <cassert>
#include <cmath>
#include "common/types.hpp"
#include "assets.hpp"
#include "utils/math.hpp"
#include "utils/raylib.hpp"
#include "constants.hpp"
#include "context.hpp"
#include "physics/joint.hpp"

/*
 * check if person should have two same body parts, i.e. one as background
 *
 * return: true if person has two same body parts
 */
static bool bodyPartIsPaired(BodyPartType t)
{
	return (int)t > (int)BodyPartType::torso_bottom;
}

/*
 * return: offset position from person to body part
 */
static Vector2 personBodyPartOffset(BodyPartType t)
{
	using enum BodyPartType;
	switch (t) {
	case head:
	        return {0, -25.5};
	case torso_top:
	        return {0, -13};
	case torso_bottom:
	        return {0, 13};
	case arm_upper:
	        return {0, -9};
	case arm_lower:
	        return {0, 5};
	case leg_upper:
	        return {0, 27};
	case leg_lower:
	        return {0, 42};
	case foot:
	        return {2, 52};
	default:
		 return {};
	}
}

/*
 * return: position for joint creation
 */
static b2Vec2 bodyPartJointPosition(const BodyPart &bp, bool top = false)
{
	b2Vec2 pos = bp.rigidbody.getPosition();
	Vector2 size = textureSize(bp.texture);
	float dy = 0.5f * size.y * PIXEL_TO_METER;
	if (!top)
		dy = -dy;
	return {
		pos.x,
		pos.y + dy
	};
}

/*
 * connect two body parts with a joint
 * @a: first body part connected to joint
 * @b: second body part connected to joint
 * @top: if true, use position at top of first body part;
 *       otherwise, use position at bottom
 */
static void bodyPartAddJoint(b2WorldId world, BodyPart &a, BodyPart &b,
			     bool top = false)
{
	b2Vec2 pos = bodyPartJointPosition(a, top);
	JointRev rev(world, a.rigidbody.id, b.rigidbody.id, pos, false);
	rev.enableSpring(true);
	rev.enableMotor(true);
	rev.setMotorSpeed(0);

	a.joints.push_back(rev);
}

/*
 * set shape filters for collision
 */
static void bodyPartSetLayers(BodyPart &bp)
{
	using enum BodyPartType;

	uint16_t cat, ex;
	switch (bp.type) {
	case head:
	case torso_top:
	case torso_middle:
	case torso_bottom:
		cat = COLLISION_CENTER;
		ex = COLLISION_ARMS | COLLISION_LEGS;
		break;
	case arm_upper:
	case arm_lower:
		cat = bp.background ? COLLISION_ARM_BG : COLLISION_ARM_FG;
		ex = bp.background ? COLLISION_ARM_FG : COLLISION_ARM_BG;
		ex |= COLLISION_CENTER;
		break;
	case leg_upper:
	case leg_lower:
	case foot:
		cat = bp.background ? COLLISION_LEG_BG : COLLISION_LEG_FG;
		ex = bp.background ? COLLISION_LEG_FG : COLLISION_LEG_BG;
		ex |= COLLISION_CENTER;
		break;
	default:
		break;
	}
	bp.rigidbody.setCollisionCategory(cat);
	bp.rigidbody.setCollisionMask(COLLISION_ALL & ~ex);
}

static float poseBodyPartHertz(PoseType p, BodyPartType bp)
{
	using enum BodyPartType;
	const PoseConfig &conf = pose_configs[(int)p];

	if (bp == torso_bottom)
		return conf.torso_bottom_frequency;
	int i = (int)bp;
	if (i < (int)arm_upper)
		return conf.base_frequency;
	else if (i < (int)leg_upper)
		return conf.arm_frequency;
	else
		return conf.leg_frequency;
}

static void bodyPartIntegrityValidate(BodyPart &bp,
				      const GameContext &cont)
{
	const Camera2D &cam = cont.camera;
	const b2ContactEvents &contacts = cont.physics.contact_events;
	const Rigidbody &rb = bp.rigidbody;

	bool visible = cameraIsVisible(cam, bodyScreenPosition(rb.id, cam));
	float volume = Normalize(cam.zoom, CAMERA_ZOOM_MIN, CAMERA_ZOOM_MAX);

	auto fn_torque_stress = [&](const JointRev &jt) {
		if (!b2RevoluteJoint_IsLimitEnabled(jt.id))
			return 0.f;

		float tor = std::fabs(jt.getTorque());
		float stress = (tor / bp.break_force_torque - 1) * GetFrameTime();
		return clamp01(stress);
	};
	auto fn_hit_valid = [&](const b2ContactHitEvent &e) {
		if (B2_ID_EQUALS(e.shapeIdA, e.shapeIdB))
			return false;
		return B2_ID_EQUALS(e.shapeIdA, rb.id) ||
			B2_ID_EQUALS(e.shapeIdB, rb.id);
	};

	float stress_max = 0;
	for (auto &jt : bp.joints) {
		if (!jt)
			continue;

		float f = fn_torque_stress(jt);
		if (f > stress_max)
			stress_max = f;
	}
	for (int i = 0; i < contacts.hitCount; i++) {
		const b2ContactHitEvent &hit = contacts.hitEvents[i];
		if (!fn_hit_valid(hit))
			continue;

		float momentum = rb.getMass() * hit.approachSpeed;
		if (momentum > bp.impact_resistance)
			bp.health -= momentum / (2 * bp.impact_resistance);
		if (bp.bone_integrity > 0 && momentum > bp.break_force)
			bp.breakBone();
	}
	if (stress_max > 0 && bp.bone_integrity > 0) {
		bp.bone_integrity = clamp01(bp.bone_integrity - stress_max);
		if (bp.bone_integrity == 0) {
			bp.breakBone();
			bp.health = 0;
			if (visible)
				asset_manager.playSound("bone_snap.mp3", volume);
		}
	}

	if (stress_max == 0 && isOpenUnitInterval(bp.bone_integrity)) {
		float f = 0.1f * GetFrameTime();
		bp.bone_integrity += bp.bone_integrity * f;
	}
}

/*
 * process the damage of each body part
 */
static void personUpdateBodyDamage(Person &per, const GameContext &cont)
{
	using enum BodyPartType;

	bool paralyze = false;
	bool heart_stopped = !per.hasHeartbeat();
	for (auto &bp : per.body_parts) {
		bodyPartIntegrityValidate(bp, cont);

		if (heart_stopped) {
			float f = 1.f / (bp.hasBrain() ? 15 : 300);
			bp.health -= f * GetFrameTime();
		}

		if (!per.alive)
			continue;

		if (bp.type == head && bp.health <= 0)
			per.kill();
		if (bp.type == torso_middle && bp.bone_integrity == 0)
			paralyze = true;

		if (paralyze)
			bp.disable();
	}

	if (heart_stopped && per.consciousness > 0) {
		float f = GetFrameTime() / 8;
		per.consciousness = clamp01(per.consciousness - f);
		if (per.consciousness == 0)
			per.setPose(PoseType::rest);
	}
}

void BodyPart::loadDefaults(BodyPartType t)
{
	using enum BodyPartType;

	type = t;
	tint = PERSON_TINT;
	health = 1;
	bone_integrity = 1;

	switch (t) {
	case head:
		impact_resistance = 100;
		break_force = 200;
		break_force_torque = 80;
		texture = asset_manager.getTexture("person_head.png");
		break;
	case torso_top:
	case torso_middle:
	case torso_bottom:
		impact_resistance = 300;
		break_force = t == torso_top ? 3000 : 1275;
		break_force_torque = 1000;
		texture = asset_manager.getTexture("person_body.png");
		break;
	case arm_upper:
	case arm_lower:
		impact_resistance = 500;
		break_force = 2000;
		break_force_torque = 400;
		texture = asset_manager.getTexture("person_limb.png");
		break;
	case leg_upper:
	case leg_lower:
		impact_resistance = 500;
		break_force = t == leg_upper ? 4000 : 2500;
		break_force_torque = 400;
		texture = asset_manager.getTexture("person_limb.png");
		break;
	case foot:
		impact_resistance = 500;
		break_force = 2000;
		break_force_torque = 200;
		texture = asset_manager.getTexture("person_foot.png");
		break;
	default:
		break;
	}
}

void BodyPart::disable()
{
	float hz = poseBodyPartHertz(PoseType::rest, type);
	float damp = pose_configs[(int)PoseType::rest].base_damping;
	for (auto &jt : joints) {
		if (!jt)
			continue;

		jt.set_hertz(hz);
		jt.set_damping_ratio(damp);
		jt.setMotorSpeed(0);
	}
}

void BodyPart::breakBone()
{
	for (auto &jt : joints) {
		if (!jt)
			continue;

		jt.enable_limit(false);
	}

	bone_integrity = 0;
}

void Person::create(b2WorldId world, Vector2 pos, bool flip)
{
	using enum BodyPartType;

	auto fn_connect = [&](BodyPart &prev, BodyPart &curr,
			      bool top = false) {
		bodyPartAddJoint(world, prev, curr, top);
	};

	alive = true;
	consciousness = 1;

	int size = (int)BodyPartType::_count;
	for (int i = 0; i < size; i++) {
		BodyPartType type = (BodyPartType)i;
		BodyPart bp{.type = type};
		Rigidbody &rb = bp.rigidbody;

		bp.loadDefaults(type);
		bp.flip = flip;

		Vector2 off = personBodyPartOffset(type);
		Vector2 bp_area = textureSize(bp.texture) * PIXEL_TO_METER;
		Vector2 bp_pos = pos + off;
		rb.initialize(world, bp_pos, bp_area, PERSON_DENSITY, flip);
		b2Body_EnableSleep(rb.id, false);
		bodyPartSetLayers(bp);

		/* connect torso with joints */
		if (i > 0 && i < 4) {
			fn_connect(body_parts[i - 1], bp, false);
		}

		body_parts.push_back(bp);

		/* add foreground body part */
		if (bodyPartIsPaired(type)) {
			BodyPart tmp = bp; // copy of current
			Rigidbody &tmp_rb = tmp.rigidbody;
			tmp.background = true;
			// re-initialize
			tmp_rb.initialize(world, bp_pos, bp_area,
					  PERSON_DENSITY, flip);
			bodyPartSetLayers(tmp);
			body_parts.push_back(tmp);
		}
	}

	/* top torso to upper arm*/
	fn_connect(body_parts[1], body_parts[4], true);
	fn_connect(body_parts[1], body_parts[5], true); // foreground
	/* upper arm to lower arm*/
	fn_connect(body_parts[4], body_parts[6]);
	fn_connect(body_parts[5], body_parts[7]); // foreground
	/* lower torso to upper leg */
	fn_connect(body_parts[3], body_parts[8]);
	fn_connect(body_parts[3], body_parts[9]); // foreground
	/* upper leg to lower leg */
	fn_connect(body_parts[8], body_parts[10]);
	fn_connect(body_parts[9], body_parts[11]); // foreground
	/* lower leg to foot */
	fn_connect(body_parts[10], body_parts[12]);
	fn_connect(body_parts[11], body_parts[13]); // foreground

	/*
	 * set and enable revolute joint limits
	 * @ind: body part joint index
	 */
	auto fn_limit = [&](BodyPart &bp, float lo, float hi, int ind = 0) {
		JointRev &rev = bp.joints[ind];
		rev.set_limits(lo, hi);
		rev.enable_limit(true);
	};

	// neck
	fn_limit(body_parts[0], -0.8, 0.8);

	/* torso */
	fn_limit(body_parts[1], -0.35, 0.35); // top
	fn_limit(body_parts[2], -0.35, 0.35); // middle

	/* shoulder */
	fn_limit(body_parts[1], -2.6, 2.6, 1);
	fn_limit(body_parts[1], -2.6, 2.6, 2);

	/* elbow */
	fn_limit(*getBodyPart(arm_upper, true), -0.1, 2);
	fn_limit(*getBodyPart(arm_upper, false), -0.1, 2);

	/* upper leg */
	fn_limit(body_parts[3], -0.25, 1.5, 0);
	fn_limit(body_parts[3], -0.25, 1.5, 1);

	/* lower leg */
	fn_limit(*getBodyPart(leg_upper, true), -2, 0.1);
	fn_limit(*getBodyPart(leg_upper, false), -2, 0.1);

	/* foot */
	fn_limit(*getBodyPart(leg_lower, true), -0.1, 0.15);
	fn_limit(*getBodyPart(leg_lower, false), -0.1, 0.15);
}

void Person::destroy()
{
	for (auto &bp : body_parts) {
		bp.rigidbody.destroy();
	}
}

void Person::update(GameContext &context)
{
	personUpdateBodyDamage(*this, context);
}

void Person::setPose(PoseType p)
{
	using enum BodyPartType;

	float damp = pose_configs[(int)p].base_damping;
	for (auto &bp : body_parts) {
		float hz = poseBodyPartHertz(p, bp.type);
		for (auto &jt : bp.joints) {
			jt.set_hertz(hz);
			jt.set_damping_ratio(damp);
		}
	}
}

void Person::kill()
{
	for (auto &bp : body_parts) {
		bp.disable();
	}

	alive = false;
}
