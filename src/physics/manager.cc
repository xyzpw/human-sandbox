#include "physics/manager.hpp"
#include <raylib.h>
#include <raymath.h>
#include <box2d/box2d.h>
#include <box2d/id.h>
#include <vector>
#include <memory>
#include "constants.hpp"
#include "utils/math.hpp"
#include "physics/rigidbody.hpp"

static void physicsPruneRigidbodies(PhysicsManager &phys)
{
	auto pred = [&](auto &a) {
		return !b2Body_IsValid(a->id);
	};

	std::erase_if(phys.rigidbodies, pred);
}

void PhysicsManager::initialize()
{
	world_def = b2DefaultWorldDef();
	world_def.gravity = vecToB2(GRAVITY * PIXEL_TO_METER);
	world_id = b2CreateWorld(&world_def);

	ground_def = b2DefaultBodyDef();
	ground_body = b2CreateBody(world_id, &ground_def);
}

void PhysicsManager::update(float dt)
{
	physicsPruneRigidbodies(*this);

	if (!paused) {
		step_accumulator += dt;
		while (step_accumulator >= PHYSICS_STEP_TIME) {
			b2World_Step(world_id, PHYSICS_STEP_TIME, 8);
			step_accumulator -= PHYSICS_STEP_TIME;
		}
		contact_events = b2World_GetContactEvents(world_id);

		for (auto &rb : rigidbodies) {
			b2Vec2 dv = rb->getVelocity() - rb->velocity_prev;

			rb->acceleration = dv / GetFrameTime();
		}
	}
}

void PhysicsManager::updateMouseJoint(b2Vec2 v)
{
	b2MouseJoint_SetTarget(mouse_joint, v);
}

void PhysicsManager::lateUpdate()
{
	for (auto &rb : rigidbodies) {
		rb->velocity_prev = rb->getVelocity();
	}
}

Rigidbody *PhysicsManager::createRigidbody(Vector2 position_px, Vector2 area,
					   float density, bool flip)
{
	auto rb = std::make_unique<Rigidbody>();
	rb->initialize(world_id, position_px, area, density, flip);

	rigidbodies.push_back(std::move(rb));
	return rigidbodies.back().get();
}

void PhysicsManager::bodyGrab(Rigidbody *rb, b2Vec2 target)
{
	b2Body_SetAwake(rb->id, true);

	b2MouseJointDef def = b2DefaultMouseJointDef();
	def.bodyIdA = ground_body;
	def.bodyIdB = rb->id;
	def.target = target;
	def.hertz = 5;
	def.dampingRatio = 0.9f;
	def.maxForce = rb->getMass() * 50 / PHYSICS_STEP_TIME;

	mouse_joint = b2CreateMouseJoint(world_id, &def);
}

void PhysicsManager::bodyRelease()
{
	b2DestroyJoint(mouse_joint);
}
