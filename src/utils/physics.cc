#include "utils/physics.hpp"
#include <box2d/box2d.h>

float bodyAngle(b2BodyId body)
{
	b2Rot r = b2Body_GetRotation(body);
	return std::atan2f(r.s, r.c);
}
