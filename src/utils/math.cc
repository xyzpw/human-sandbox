#include "utils/math.hpp"
#include <raylib.h>
#include <raymath.h>
#include <box2d/box2d.h>

float clamp01(float f)
{
	return Clamp(f, 0, 1);
}

b2Vec2 vecToB2(Vector2 v)
{
	return (b2Vec2){v.x, -v.y};
}

Vector2 b2ToVec(b2Vec2 v)
{
	return (Vector2){v.x, -v.y};
}
