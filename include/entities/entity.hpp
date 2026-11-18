#pragma once

#include <raylib.h>
#include "physics/rigidbody.hpp"

struct Entity {
	Texture2D texture;
	Color tint;
	Rigidbody *rigidbody = nullptr;
	bool flip = false;
};
