#include "effects/camera.hpp"
#include <raylib.h>
#include <raymath.h>
#include <cmath>

void CameraShaker::update(Camera2D &cam, float dt)
{
	if (!active)
		return;

	time_left = Clamp(time_left - dt, 0, time_left);

	if (time_left == 0) {
		cam.offset = offset_base;
		active = false;
		return;
	}

	float t = duration - time_left;
	float dy = std::sinf(2 * PI * frequency * t) * amplitude;
	cam.offset.y = offset_base.y + dy;
}
