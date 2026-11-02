#pragma once

#include <raylib.h>
#include <box2d/box2d.h>
#include "rigidbody.hpp"

constexpr float PHYSICS_STEP_TIME = 1 / 60.0f;

struct PhysicsManager {
	b2WorldDef world_def;
	b2WorldId world_id;

	b2BodyDef ground_def;
	b2BodyId ground_body;
	b2JointId mouse_joint;

	b2ContactEvents contact_events;

	bool paused = false;

	float step_accumulator = 0;

	void initialize();

	void update(float dt);
	void updateMouseJoint(b2Vec2 v);

	/*
	 * grab onto body for dragging
	 */
	void bodyGrab(Rigidbody *rb, b2Vec2 target);
	/*
	 * release held body
	 */
	void bodyRelease();
};
