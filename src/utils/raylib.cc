#include "utils/raylib.hpp"
#include <raylib.h>
#include <raymath.h>

Vector2 textureSize(Texture2D tex)
{
	return {(float)tex.width, (float)tex.height};
}

bool cameraIsVisible(const Camera2D &cam, Vector2 pos)
{
	Rectangle camrec = {
		0.f, 0.f,
		(float)GetScreenWidth(),
		(float)GetScreenHeight()
	};
	return CheckCollisionPointRec(pos, camrec);
}
