#pragma once

#include <raylib.h>

constexpr float METER_TO_PIXEL = 50;
constexpr float PIXEL_TO_METER = 1 / METER_TO_PIXEL;
constexpr Vector2 GRAVITY = {0, 9.80665f * METER_TO_PIXEL};

constexpr int WINDOW_WIDTH = 1120;
constexpr int WINDOW_HEIGHT = 630;

constexpr float CAMERA_ZOOM_MIN = 0.1f;
constexpr float CAMERA_ZOOM_MAX = 10;
