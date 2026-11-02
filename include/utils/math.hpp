#pragma once

#include <raylib.h>
#include <box2d/box2d.h>

float clamp01(float f);

/*
 * check if rectangle is inside another
 * @rect: the larger rectangle
 * @smaller: the rectangle which should be inside the larger one
 *
 * return: true if small rectangle is inside larger
 */
bool rectIsInside(Rectangle rect, Rectangle smaller);

/*
 * return: width and height of texture
 */
Vector2 textureSize(Texture2D tex);

b2Vec2 vecToB2(Vector2 v);
Vector2 b2ToVec(b2Vec2 v);

float bodyAngle(b2BodyId body);
