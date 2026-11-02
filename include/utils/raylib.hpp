#pragma once

#include <raylib.h>

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
