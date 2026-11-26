#pragma once

#include <raylib.h>
#include <box2d/box2d.h>
#include <vector>
#include <memory>
#include "rigidbody.hpp"

constexpr float PHYSICS_STEP_TIME = 1 / 60.0f;

struct PhysicsManager {
	b2WorldDef world_def;
	b2WorldId world_id;

	b2BodyDef ground_def;
	b2BodyId ground_body;
	b2BodyId mouse_body;
	b2JointId mouse_joint;

	b2ContactEvents contact_events;
	std::vector<std::unique_ptr<Rigidbody>> rigidbodies{};

	bool paused = false;

	float step_accumulator = 0;

	void initialize();

	void update(float dt);
	void updateMouseJoint(b2Vec2 v);
	void lateUpdate();

	/*
	 * create rigidbody
	 * @position_px: world position in pixels
	 * @area: size of shape in meters
	 * @density: shape density
	 */
	Rigidbody *createRigidbody(Vector2 position_px, Vector2 area,
				   float density, bool flip = false);

	/*
	 * grab onto body for dragging
	 */
	void bodyGrab(Rigidbody *rb, b2Vec2 target);
	/*
	 * release held body
	 */
	void bodyRelease();
};
