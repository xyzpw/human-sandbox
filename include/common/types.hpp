#pragma once

#include <cstdint>

enum class SpawnableType {
	person,
	cube_metal,
	_count,
};

enum class BodyPartType {
	head,
	torso_top,
	torso_middle,
	torso_bottom,
	arm_upper, arm_lower,
	leg_upper, leg_lower,
	foot,
	_count
};

enum class PoseType {
	rest,
	standing,
	_count
};

enum CollisionCategory : uint16_t {
	COLLISION_CENTER     = 0x0001, // between foreground and background
	COLLISION_ARM_BG     = 0x0002,
	COLLISION_ARM_FG     = 0x0004,
	COLLISION_LEG_BG     = 0x0008,
	COLLISION_LEG_FG     = 0x0010,
};

enum ViewFlags : uint16_t {
	VIEW_FLAGS_FPS      = 0x0001,
	VIEW_FLAGS_DETAILED = 0x0002,
};

// includes all collision categories
constexpr uint16_t COLLISION_ALL = COLLISION_CENTER
				 | COLLISION_ARM_BG
				 | COLLISION_ARM_FG
				 | COLLISION_LEG_BG
				 | COLLISION_LEG_FG;
// includes background and foreground collision
constexpr uint16_t COLLISION_ARMS = COLLISION_ARM_BG | COLLISION_ARM_BG;
// includes background and foreground collision
constexpr uint16_t COLLISION_LEGS = COLLISION_LEG_BG | COLLISION_LEG_FG;
