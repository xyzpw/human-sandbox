#pragma once

#include <raylib.h>
#include <box2d/box2d.h>
#include "physics/rigidbody.hpp"

struct World;
struct PhysicsManager;

struct InputManager {
	Vector2 mouse;
	Vector2 mouse_world;
	b2Vec2 mouse_b2;

	b2JointId mouse_joint;
	Rigidbody *held_body = nullptr;

	void update(Camera2D &cam, float dt);
	void updateHolding(const World &world, PhysicsManager &phys);

	void bodyTryGrab(World &world, PhysicsManager &phys);
	void bodyRelease(PhysicsManager &phys);

	void heldRotate(float f);
};
