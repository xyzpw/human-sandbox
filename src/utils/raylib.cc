#include "utils/raylib.hpp"
#include "utils/math.hpp"
#include <raylib.h>
#include <raymath.h>
#include <box2d/box2d.h>
#include "constants.hpp"

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

Vector2 bodyScreenPosition(b2BodyId id, Camera2D cam)
{
	Vector2 pos = b2ToVec(b2Body_GetPosition(id)) * METER_TO_PIXEL;
	return GetWorldToScreen2D(pos, cam);
}
