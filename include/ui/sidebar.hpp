#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include "entities/spawnable.hpp"

struct World;

struct SidebarButton {
	enum class Type {
		clear_all,
		clear_living,
		_count
	};

	Type type;

	Rectangle area;
	std::string text;
	const float fontsize = 11;
	Vector2 text_position;
};

struct SidebarItem {
	Rectangle area;
	Spawnable spawnable;
};

struct Sidebar {
	Rectangle area;
	Color color, color_button;

	std::vector<SidebarItem> items;
	std::vector<SidebarButton> buttons;
	int selected_index = 0;

	float width_closed, width_open;
	bool is_open = true;

	void initialize();

	/*
	 * select current spawnable item under mouse
	 */
	void clickMouse(Vector2 mouse, World &world);

	void toggleOpen()
	{
		if (is_open)
			close();
		else
			open();
	}
	void open()
	{
		area.width = 0.25f * GetScreenWidth();
		area.height = GetScreenHeight();

		is_open = true;
	}
	void close()
	{
		area.width = 10;

		is_open = false;
	}

	/*
	 * return: selected item from sidebar
	 */
	const SidebarItem &getSelectedItem() const
	{
		return items.at(selected_index);
	}

	/*
	 * draw sidebar and items
	 */
	void draw() const;
};
