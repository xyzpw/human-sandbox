#include "ui/sidebar.hpp"
#include "world.hpp"
#include <raylib.h>
#include <raymath.h>
#include "entities/spawnable.hpp"
#include "entities/person.hpp"
#include "assets.hpp"

static Spawnable createSpawnable(SpawnableType t)
{
	using enum SpawnableType;

	Spawnable result{.type = t};
	switch (t) {
	case person:
		result.name = "Person";
		result.texture_name = "person_head.png";
		result.tint = PERSON_TINT;
		break;
	case cube_metal:
		result.name = "Metal Cube";
		result.texture_name = "cube.png";
		result.tint = {181, 183, 185, 255};
		break;
	default:
		break;
	}
	return result;
}

static void sidebarAddButtons(Sidebar &sb)
{
	static const Font font = GetFontDefault();

	using Type = SidebarButton::Type;

	int btn_count = (int)Type::_count;

	float spacing_y = 3;
	float x = 3;
	float width = sb.width_open - x * 2;
	float height = 24;
	float y = (btn_count + 1) * height + (btn_count - 1) * spacing_y;
	y = (GetScreenHeight() - spacing_y) - y;

	for (int i = 0; i < btn_count; ++i) {
		Type type = (Type)i;
		y += height + spacing_y;

		SidebarButton btn{.type = type};
		btn.area = {x, y, width, height};

		Vector2 text_size{};

		switch (type) {
		case Type::clear_all:
			btn.text = "Clear All";
			break;
		case Type::clear_living:
			btn.text = "Clear Living";
			break;
		default:
			continue;
		}

		text_size = MeasureTextEx(font, btn.text.c_str(), btn.fontsize, 0);
		btn.text_position = {
			x + 0.5f * (width - text_size.x),
			y + 0.5f * (height - text_size.y)
		};

		sb.buttons.push_back(btn);
	}
}

static void sidebarAddSpawnables(Sidebar *sb)
{
	using enum SpawnableType;

	int size = (int)SpawnableType::_count;
	for (int i = 0; i < size; i++) {
		SpawnableType type = (SpawnableType)i;

		float x = 25 + 32.f * i + (i == 0 ? 0 : 5);
		SidebarItem item = {
			.area = {x, 25.f, 32.f, 32.f},
			.spawnable = createSpawnable(type)
		};

		sb->items.push_back(item);
	}
}

void Sidebar::initialize()
{
	width_closed = 10;
	width_open = 0.25f * GetScreenWidth();
	area = {
		.x = 0.f,
		.y = 0.f,
		.width = width_open,
		.height = (float)GetScreenHeight()
	};
	color = ColorAlpha(BLACK, 1.0f / 6);
	color_button = ColorAlpha(BLACK, 0.5f);

	sidebarAddButtons(*this);
	sidebarAddSpawnables(this);
}

void Sidebar::clickMouse(Vector2 mouse, World &world)
{
	using Type = SidebarButton::Type;

	int i = -1;
	for (const auto &it : items) {
		i++;

		if (CheckCollisionPointRec(mouse, it.area)) {
			selected_index = i;
			return;
		}
	}

	for (const auto &btn : buttons) {
		if (!CheckCollisionPointRec(mouse, btn.area))
			continue;

		if (btn.type == Type::clear_all)
			world.clearItems();
		if (btn.type == Type::clear_living)
			world.clearLiving();
	}
}

void Sidebar::draw() const
{
	DrawRectangleRec(area, color);

	if (!is_open)
		return;

	int i = -1;
	for (const auto &it : items) {
		i++;

		const Spawnable &s = it.spawnable;
		const Texture2D &tex =
			asset_manager.getTexture(s.texture_name);

		Rectangle src{0.f, 0.f, (float)tex.width, (float)tex.height};
		DrawTexturePro(tex, src, it.area, Vector2Zeros, 0,
			       it.spawnable.tint);

		if (selected_index == i) {
			DrawRectangleLines(it.area.x, it.area.y,
					   it.area.width, it.area.height,
					   GREEN);
		}
	}

	for (const auto &btn : buttons) {
		DrawRectangleRec(btn.area, color_button);
		DrawText(btn.text.c_str(),
			 btn.text_position.x, btn.text_position.y,
			 btn.fontsize, WHITE);
	}
}
