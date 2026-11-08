#include "utils/physics.hpp"
#include <box2d/box2d.h>

float bodyAngle(b2BodyId body)
{
	b2Rot r = b2Body_GetRotation(body);
	return std::atan2f(r.s, r.c);
}

b2BodyType shapeBodyType(b2ShapeId s)
{
	return b2Body_GetType(b2Shape_GetBody(s));
}
