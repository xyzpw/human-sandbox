#include "effects.hpp"
#include <raylib.h>
#include <box2d/box2d.h>
#include "effects/camera.hpp"
#include "context.hpp"
#include "utils/physics.hpp"
#include "utils/raylib.hpp"

static bool cameraShouldShake(const b2ContactHitEvent &hit)
{
	b2BodyType type_a = shapeBodyType(hit.shapeIdA);
	b2BodyType type_b = shapeBodyType(hit.shapeIdB);
	if (type_a == type_b)
		return false;

	float mass = type_b == b2_staticBody
		? b2Body_GetMass(b2Shape_GetBody(hit.shapeIdA))
		: b2Body_GetMass(b2Shape_GetBody(hit.shapeIdB));
	return mass * hit.approachSpeed > 100;
}

void effectsProcessCollision(GameContext &context)
{
	Camera2D &cam = context.camera;
	CameraShaker &shaker = context.camera_shaker;
	const b2ContactEvents &contacts = context.physics.contact_events;

	for (int i = 0; i < contacts.hitCount; i++) {
		const b2ContactHitEvent &hit = contacts.hitEvents[i];
		if (B2_ID_EQUALS(hit.shapeIdA, hit.shapeIdB))
			continue;

		Vector2 pos_screen = vecScreenPosition(hit.point, cam);
		bool visible = cameraIsVisible(cam, pos_screen);

		if (visible && !shaker.active && cam.zoom > 1 &&
		    cameraShouldShake(hit)) {
			shaker.initialize(cam.offset, 0.05f, 10, 5);
		}
	}
}
