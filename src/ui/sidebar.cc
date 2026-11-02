#include "ui/sidebar.hpp"
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

	sidebarAddSpawnables(this);
}

void Sidebar::clickMouse(Vector2 mouse)
{
	int i = -1;
	for (const auto &it : items) {
		i++;

		if (CheckCollisionPointRec(mouse, it.area)) {
			selected_index = i;
			return;
		}
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
}
