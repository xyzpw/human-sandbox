#include "utils/display.hpp"
#include <raylib.h>
#include <raymath.h>
#include "utils/math.hpp"

void drawTextureCenter(Texture2D tex, Vector2 point, float rotation,
		       bool flip, Color tint)
{
	Vector2 size = textureSize(tex);
	Rectangle src = {0.f, 0.f, size.x, size.y};
	Rectangle dst = {point.x, point.y, size.x, size.y};
	if (flip) {
		src.height = -src.height;
	}
	DrawTexturePro(tex, src, dst, size * 0.5f, rotation, tint);
}
