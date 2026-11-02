#pragma once

#include <string>
#include <raylib.h>
#include "common/types.hpp"

struct Spawnable {
	SpawnableType type;
	std::string name;
	std::string texture_name;
	Color tint;
};
