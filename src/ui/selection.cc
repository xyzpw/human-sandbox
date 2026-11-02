#include "ui/selection.hpp"
#include <raylib.h>
#include <raymath.h>
#include <algorithm>
#include <cmath>
#include "constants.hpp"

Rectangle SelectionBox::getRectangle(Vector2 mouse) const
{
	Rectangle result{
		.x = std::min(mouse.x, point_start.x),
		.y = std::min(mouse.y, point_start.y),
	};
	result.width = std::fabs(mouse.x - point_start.x);
	result.height = std::fabs(mouse.y - point_start.y);

	return result;
}

void SelectionBox::draw(Vector2 mouse, float zoom) const
{
	Rectangle rec = getRectangle(mouse);
	DrawRectangleRec(rec, SELECTION_BOX_COLOR);
	DrawRectangleLines(rec.x, rec.y, rec.width, rec.height,
			   SELECTION_BOX_LINES_COLOR);

	drawText(rec, zoom);
}

void SelectionBox::drawText(Rectangle rec, float zoom) const
{
	static Font font = GetFontDefault();
	static float fontsize = 16;
	static float offset = fontsize;
	static float spacing = 3;

	float w = rec.width * PIXEL_TO_METER / zoom;
	float h = rec.height * PIXEL_TO_METER / zoom;

	if (w < 0.1 && h < 0.1)
		return;

	const char *text_width = TextFormat("%.1f m", w);
	const char *text_height = TextFormat("%.1f m", h);

	float text_w_length = 0.6f * fontsize * TextLength(text_height);
	float text_h_length = 0.6f * fontsize * TextLength(text_width);
	Vector2 w_pos = {
		rec.x + 0.5f * (rec.width - text_w_length),
		rec.y - offset
	};
	Vector2 h_pos = {
		rec.x - offset,
		rec.y + 0.5f * (rec.height + text_h_length)
	};

	DrawTextPro(font, text_width, w_pos, Vector2Zeros, 0, fontsize,
		    spacing, SELECTION_BOX_TEXT_COLOR);
	DrawTextPro(font, text_height, h_pos, Vector2Zeros, -90, fontsize,
		    spacing, SELECTION_BOX_TEXT_COLOR);
}
