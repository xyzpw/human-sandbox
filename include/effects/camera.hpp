#pragma once

#include <raylib.h>

struct CameraShaker {
	Vector2 offset_base;
	float duration, frequency, amplitude;
	bool active = false;
	float time_left;

	/*
	 * @offset: baseline camera offset
	 * @duration: duration of effect
	 * @n: oscillations
	 * @amp: amplitude of sine wave
	 */
	void initialize(Vector2 offset, float duration, float n, float amp)
	{
		offset_base = offset;
		this->duration = duration;
		time_left = duration;
		frequency = n / duration;
		amplitude = amp;
		active = true;
	}

	void update(Camera2D &cam, float dt);
};
