#include <raylib.h>
#include "constants.hpp"
#include "game.hpp"

int main()
{
	SetTraceLogLevel(LOG_WARNING);
	SetConfigFlags(FLAG_VSYNC_HINT);

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sandbox");
	InitAudioDevice();

	Game game;
	game.initialize();

	while (!WindowShouldClose()) {
		float dt = GetFrameTime();

		game.update(dt);

		BeginDrawing();
		BeginMode2D(game.context.camera);

		ClearBackground(BLACK);
		game.draw();

		EndMode2D();

		game.drawOverlay();

		EndDrawing();
	}

	game.unload();
	CloseAudioDevice();
	CloseWindow();
	return 0;
}
