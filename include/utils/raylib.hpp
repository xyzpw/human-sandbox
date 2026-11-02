#pragma once

#include <raylib.h>


/*
 * check if position is visible
 * @pos: screen position
 *
 * return: true if position is visible to camera
 */
bool cameraIsVisible(const Camera2D &cam, Vector2 pos);
