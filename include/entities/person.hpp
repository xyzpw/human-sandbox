#pragma once

#include <vector>
#include <raylib.h>
#include <raymath.h>
#include <box2d/box2d.h>
#include "entity.hpp"
#include "common/types.hpp"
#include "physics/joint.hpp"
#include "constants.hpp"

struct GameContext;

constexpr Color PERSON_TINT = {190, 190, 190, 255};
constexpr float PERSON_DENSITY = 70.f /
	(11 * 89 * PIXEL_TO_METER * PIXEL_TO_METER);

struct PoseConfig {
	float base_frequency;
	float arm_frequency;
	float torso_bottom_frequency;
	float leg_frequency;
	float base_damping;
};

constexpr PoseConfig pose_configs[(int)PoseType::_count] = {
	(PoseConfig){
		.base_frequency = 0.23,
		.arm_frequency = 0.2,
		.torso_bottom_frequency = 1,
		.leg_frequency = 0.6,
		.base_damping = 0.85,
	},
	(PoseConfig){
		.base_frequency = 6,
		.arm_frequency = 7,
		.torso_bottom_frequency = 7,
		.leg_frequency = 10,
		.base_damping = 0.97,
	}
};

struct BodyPart : Entity {
	BodyPartType type;
	std::vector<JointRev> joints;
	bool background;

	float impact_resistance;
	// force required to break bone
	float break_force;
	// torque required to break bone
	float break_force_torque;

	float health;
	float bone_integrity;

	/*
	 * load default data, e.g. constants
	 */
	void loadDefaults(BodyPartType t);

	void disable();
	void breakBone();

	float healthLowest() const
	{
		return health < bone_integrity ? health : bone_integrity;
	}
};

struct Person {
	std::vector<BodyPart> body_parts;

	bool alive;

	void create(b2WorldId world, Vector2 pos, bool flip = false);
	void destroy();

	void update(GameContext &context);

	void setPose(PoseType t);

	void kill();

	BodyPart *getBodyPart(BodyPartType t, bool background)
	{
		for (auto &p : body_parts) {
			if (p.type == t && p.background == background)
				return &p;
		}
		return nullptr;
	}
};
