#pragma once

#include <raylib.h>
#include "physics/manager.hpp"
#include "input/manager.hpp"
#include "effects/camera.hpp"

struct GameContext {
	Camera2D camera;
	CameraShaker camera_shaker;
	PhysicsManager physics;
	InputManager input;
};
