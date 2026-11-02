#include "utils/raylib.hpp"
#include <raylib.h>
#include <raymath.h>

bool cameraIsVisible(const Camera2D &cam, Vector2 pos)
{
	Rectangle camrec = {
		0.f, 0.f,
		(float)GetScreenWidth(),
		(float)GetScreenHeight()
	};
	return CheckCollisionPointRec(pos, camrec);
}
