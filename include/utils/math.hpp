#pragma once

#include <raylib.h>
#include <box2d/box2d.h>

float clamp01(float f);

/*
 * return: width and height of texture
 */
Vector2 textureSize(Texture2D tex);

b2Vec2 vecToB2(Vector2 v);
Vector2 b2ToVec(b2Vec2 v);

float bodyAngle(b2BodyId body);
