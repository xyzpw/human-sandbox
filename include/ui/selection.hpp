#pragma once

#include <raylib.h>

constexpr Color SELECTION_BOX_TEXT_COLOR = {255, 0, 102, 255};
constexpr Color SELECTION_BOX_COLOR = {255, 0, 102, 51};
constexpr Color SELECTION_BOX_LINES_COLOR = {255, 0, 102, 153};

struct SelectionBox {
	Vector2 point_start{};
	bool active = false;

	/*
	 * compute rectangle for display
	 */
	Rectangle getRectangle(Vector2 mouse) const;

	/*
	 * draw selection box and text
	 * @mouse: current mouse screen position
	 * @zoom: current camera zoom
	 */
	void draw(Vector2 mouse, float zoom) const;

	private:
	void drawText(Rectangle rec, float zoom) const;
};
