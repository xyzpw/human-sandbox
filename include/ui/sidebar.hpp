#pragma once

#include <raylib.h>
#include <vector>
#include "entities/spawnable.hpp"

struct SidebarItem {
	Rectangle area;
	Spawnable spawnable;
};

struct Sidebar {
	Rectangle area;
	Color color;

	std::vector<SidebarItem> items;
	int selected_index = 0;

	float width_closed, width_open;
	bool is_open = true;

	void initialize();

	/*
	 * select current spawnable item under mouse
	 */
	void clickMouse(Vector2 mouse);

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
