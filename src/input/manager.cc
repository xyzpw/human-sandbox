#include "input/manager.hpp"
#include <raylib.h>
#include <raymath.h>
#include <box2d/box2d.h>
#include "world.hpp"
#include "utils/math.hpp"
#include "physics/manager.hpp"

void InputManager::update(Camera2D &cam, float dt)
{
	mouse = GetMousePosition();
	mouse_world = GetScreenToWorld2D(mouse, cam);
	mouse_b2 = vecToB2(mouse_world * PIXEL_TO_METER);

	if (held_body) {
		b2Body_SetAwake(held_body->id, true);
	}
}

void InputManager::updateHolding(const World &world, PhysicsManager &phys)
{
	if (!held_body)
		return;

	if (!phys.paused) {
		phys.updateMouseJoint(mouse_b2);
		return;
	}

	if (!CheckCollisionPointRec(GetMousePosition(), world.background_area))
		return;

	Vector2 next = (mouse_world + GetMouseDelta()) * PIXEL_TO_METER;
	held_body->setPosition(vecToB2(next));
}

void InputManager::bodyTryGrab(World &world, PhysicsManager &phys)
{
	if (held_body)
		return;

	Rigidbody *rb = world.getBodyAt(mouse_world);
	if (!rb)
		return;

	phys.bodyGrab(rb, mouse_b2);
	held_body = rb;
}

void InputManager::bodyRelease(PhysicsManager &phys)
{
	phys.bodyRelease();
	b2Body_SetFixedRotation(held_body->id, false);
	held_body = nullptr;
}

void InputManager::heldRotate(float f)
{
	if (!held_body)
		return;

	b2Body_SetFixedRotation(held_body->id, true);

	b2Rot next = b2MakeRot(bodyAngle(held_body->id) + f);
	held_body->setRotation(next);
}
