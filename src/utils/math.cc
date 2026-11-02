#include "utils/math.hpp"
#include <raylib.h>
#include <raymath.h>
#include <box2d/box2d.h>
#include <cmath>

float clamp01(float f)
{
	return Clamp(f, 0, 1);
}

bool rectIsInside(Rectangle rect, Rectangle smaller)
{
	bool fail = smaller.x < rect.x;
	fail |= smaller.x + smaller.width > rect.x + rect.width;
	fail |= smaller.y < rect.y;
	fail |= smaller.y + smaller.height > rect.y + rect.height;

	return !fail;
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
