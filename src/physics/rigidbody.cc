#include "physics/rigidbody.hpp"
#include <raylib.h>
#include <raymath.h>
#include <box2d/box2d.h>
#include "utils/math.hpp"
#include "constants.hpp"
#include "common/types.hpp"

void Rigidbody::initialize(b2WorldId world, Vector2 position,
			   Vector2 area, float density, bool flip)
{
	def = b2DefaultBodyDef();
	def.position = vecToB2(position * PIXEL_TO_METER);
	def.rotation = b2MakeRot(flip ? PI : 0);
	def.type = b2_dynamicBody;
	id = b2CreateBody(world, &def);

	b2Polygon box = b2MakeBox(0.5f * area.x, 0.5f * area.y);
	shape_def = b2DefaultShapeDef();
	shape_def.filter.categoryBits = COLLISION_ALL;
	shape_def.filter.maskBits = COLLISION_ALL;
	shape_def.density = density;
	shape_def.enableContactEvents = true;
	shape_def.enableHitEvents = true;
	shape_id = b2CreatePolygonShape(id, &shape_def, &box);
}
