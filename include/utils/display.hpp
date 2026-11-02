#pragma once

#include <raylib.h>

/*
 * draw a texture positioned and rotated around its center
 * @point: point at center of texture
 * @rotation: rotation (degrees) at center origin
 * @flip: flip texture (use when spawning in negative direction)
 */
void drawTextureCenter(Texture2D tex, Vector2 point, float rotation,
		       bool flip = false,
		       Color tint = WHITE);
