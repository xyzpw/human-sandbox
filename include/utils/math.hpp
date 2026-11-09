#pragma once

#include <raylib.h>
#include <box2d/box2d.h>

float clamp01(float f);

/*
 * check if a value is greater than 0 and less than 1
 */
bool isOpenUnitInterval(float f);

b2Vec2 vecToB2(Vector2 v);
Vector2 b2ToVec(b2Vec2 v);
