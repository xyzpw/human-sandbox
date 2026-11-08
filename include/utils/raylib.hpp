#pragma once

#include <raylib.h>
#include <box2d/box2d.h>

/*
 * return: width and height of texture
 */
Vector2 textureSize(Texture2D tex);

/*
 * check if position is visible
 * @pos: screen position
 *
 * return: true if position is visible to camera
 */
bool cameraIsVisible(const Camera2D &cam, Vector2 pos);

/*
 * get screen position of box2d vector
 *
 * NOTE: vector will be scaled from meters to pixels
 *
 * return: world to screen position
 */
Vector2 vecScreenPosition(b2Vec2 v, Camera2D cam);
Vector2 bodyScreenPosition(b2BodyId id, Camera2D cam);
