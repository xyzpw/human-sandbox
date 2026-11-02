#include "utils/math.hpp"
#include <raylib.h>
#include <raymath.h>
#include <box2d/box2d.h>
#include <cmath>

float clamp01(float f)
{
	return Clamp(f, 0, 1);
}

Vector2 textureSize(Texture2D tex)
{
	return {(float)tex.width, (float)tex.height};
}

b2Vec2 vecToB2(Vector2 v)
{
	return (b2Vec2){v.x, -v.y};
}

Vector2 b2ToVec(b2Vec2 v)
{
	return (Vector2){v.x, -v.y};
}

float bodyAngle(b2BodyId body)
{
	b2Rot r = b2Body_GetRotation(body);
	return std::atan2f(r.s, r.c);
}
