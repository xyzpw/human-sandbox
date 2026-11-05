#pragma once

#include <raylib.h>
#include <box2d/box2d.h>
#include "world.hpp"
#include "input/manager.hpp"
#include "ui/selection.hpp"
#include "ui/sidebar.hpp"
#include "context.hpp"

struct Game {
	GameContext context;

	SelectionBox selection;
	Sidebar sidebar;

	World world;

	uint16_t view_bits = 0;

	void initialize();
	void unload();

	void update(float dt);

	void draw() const;
	void drawOverlay() const;

	/*
	 * move camera opposite to mouse delta, scaled by zoom
	 * @delta: delta mouse position
	 */
	void cameraMove(Vector2 delta);
	/*
	 * zoom camera
	 * @delta: delta mouse wheel
	 */
	void cameraZoom(float delta);

	private:
	void handleInputCamera();
	void handleInputSelection();
	void handleInputInteraction(float dt);
};
