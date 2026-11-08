#include "game.hpp"
#include <raylib.h>
#include <raymath.h>
#include <box2d/box2d.h>
#include "constants.hpp"
#include "common/names.hpp"
#include "assets.hpp"
#include "toast.hpp"
#include "ui/sidebar.hpp"
#include "utils/raylib.hpp"
#include "utils/physics.hpp"

static void drawFlagDetailed(const World &world, Camera2D cam)
{
	static auto fn_col = [](float hp) {
		hp = Clamp(hp, 0, 1);
		unsigned char r = 255 * (1 - hp);
		unsigned char g = 255 * hp;
		return (Color){r, g, 0, 255};
	};

	for (const auto &it : world.items) {
		if (!it.isType<Person>())
			continue;

		const Person &per = *it.get_if<Person>();

		for (const auto &bp : per.body_parts) {
			Vector2 pos = bodyScreenPosition(bp.rigidbody.id, cam);
			DrawCircleV(pos, 2, fn_col(bp.bone_integrity));
		}
	}
}

static void checkCameraShake(GameContext &context)
{
	if (context.camera.zoom < 1)
		return;

	Camera2D &cam = context.camera;
	const b2ContactEvents &contacts = context.physics.contact_events;

	for (int i = 0; i < contacts.hitCount; i++) {
		const b2ContactHitEvent &hit = contacts.hitEvents[i];
		Vector2 pos = vecScreenPosition(hit.point, cam);
		if (B2_ID_EQUALS(hit.shapeIdA, hit.shapeIdB) ||
		    !cameraIsVisible(cam, pos))
			continue;

		b2BodyType type_a = shapeBodyType(hit.shapeIdA);
		b2BodyType type_b = shapeBodyType(hit.shapeIdB);
		if (type_a == type_b)
			continue;

		float mass = type_b == b2_staticBody
			? b2Body_GetMass(b2Shape_GetBody(hit.shapeIdA))
			: b2Body_GetMass(b2Shape_GetBody(hit.shapeIdB));
		float momentum = mass * hit.approachSpeed;
		if (momentum < 100)
			continue;

		context.camera_shaker.initialize(cam.offset, 0.05f, 10, 5);
		return;
	}
}

void Game::initialize()
{
	PhysicsManager &physics = context.physics;

	asset_manager.load();
	physics.initialize();
	world.initialize(physics.world_id);
	world.clearItems();
	sidebar.initialize();

	context.camera = Camera2D{
		.offset = {
			0.5f * GetScreenWidth(),
			0.5f * GetScreenHeight()
		},
		.target = world.getStartPoint(),
		.rotation = 0,
		.zoom = 1
	};

	Music amb = asset_manager.getMusic(MUSIC_AMBIENCE_NAME);
	SetMusicVolume(amb, 0.1f);
	PlayMusicStream(amb);
}

void Game::unload()
{
	Music amb = asset_manager.getMusic(MUSIC_AMBIENCE_NAME);
	if (IsMusicStreamPlaying(amb))
		StopMusicStream(amb);

	asset_manager.unload();
	b2DestroyWorld(context.physics.world_id);
}

void Game::update(float dt)
{
	PhysicsManager &physics = context.physics;
	InputManager &input = context.input;

	input.update(context.camera, dt);
	handleInputCamera();
	handleInputInteraction(dt);
	handleInputSelection();
	input.updateHolding(world, physics);
	world.update(context);
	physics.update(dt);
	context.camera_shaker.update(context.camera, dt);
	toast_manager.update(dt);
	UpdateMusicStream(asset_manager.getMusic(MUSIC_AMBIENCE_NAME));

	checkCameraShake(context);

	if (IsKeyPressed(KEY_GRAVE))
		view_bits ^= VIEW_FLAGS_FPS;
	if (IsKeyPressed(KEY_S))
		view_bits ^= VIEW_FLAGS_DETAILED;
	if (IsKeyPressed(KEY_SPACE)) {
		physics.paused = !physics.paused;
		const Music &amb = asset_manager.getMusic(MUSIC_AMBIENCE_NAME);
		if (physics.paused)
			PauseMusicStream(amb);
		else
			PlayMusicStream(amb);
	}
}

void Game::draw() const
{
	world.drawBackground();
	world.drawForeground();
}

void Game::drawOverlay() const
{
	if (selection.active)
		selection.draw(GetMousePosition(), context.camera.zoom);
	if (!toast_manager.notifications.empty())
		toast_manager.drawOverlay();

	sidebar.draw();

	if (view_bits & VIEW_FLAGS_FPS) {
		static int x = GetScreenWidth() - 90;
		DrawFPS(x, 10);
	}

	if (view_bits & VIEW_FLAGS_DETAILED)
		drawFlagDetailed(world, context.camera);
}

void Game::cameraMove(Vector2 delta)
{
	context.camera.target -= delta / context.camera.zoom;
}

void Game::cameraZoom(float delta)
{
	Camera2D &cam = context.camera;

	cam.zoom += delta * 0.1f * cam.zoom;
	cam.zoom = Clamp(cam.zoom, CAMERA_ZOOM_MIN, CAMERA_ZOOM_MAX);
}

void Game::handleInputCamera()
{
	Vector2 mouse_delta = GetMouseDelta();
	int wheel_delta = GetMouseWheelMove();

	if (wheel_delta) {
		cameraZoom(wheel_delta);
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
		cameraMove(mouse_delta);
	}
}

void Game::handleInputSelection()
{
	Vector2 mouse = GetMousePosition();
	bool left_down = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	bool left_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	bool hovering_sidebar = CheckCollisionPointRec(mouse, sidebar.area);

	if (left_pressed && !selection.active && !hovering_sidebar &&
	    !context.input.held_body) {
		selection.active = true;
		selection.point_start = mouse;
	}

	if (!left_down && selection.active)
		selection.active = false;

	if (IsKeyPressed(KEY_TAB))
		sidebar.toggleOpen();

	/* select an item from sidebar */
	if (hovering_sidebar && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		sidebar.clickMouse(mouse);
}

void Game::handleInputInteraction(float dt)
{
	InputManager &input = context.input;
	PhysicsManager &phys = context.physics;

	Vector2 mouse = GetMousePosition();
	bool sidebar_hovering = CheckCollisionPointRec(mouse, sidebar.area);

	int spawn_dir = (int)IsKeyPressed(KEY_E) - (int)IsKeyPressed(KEY_Q);
	if (spawn_dir != 0) {
		const SidebarItem &s = sidebar.getSelectedItem();
		Vector2 v = GetScreenToWorld2D(GetMousePosition(),
					       context.camera);
		bool flip = spawn_dir == -1;
		world.spawnItem(phys.world_id, s.spawnable, v, flip);
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !sidebar_hovering) {
		input.bodyTryGrab(world, phys);
	}
	if (input.held_body && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		input.bodyRelease(phys);
	}

	float rot_dir = (float)IsKeyDown(KEY_A) - (float)IsKeyDown(KEY_D);
	if (rot_dir != 0) {
		float f = 135;
		if (IsKeyDown(KEY_LEFT_SHIFT))
			f += 45;
		if (IsKeyDown(KEY_LEFT_CONTROL))
			f -= 45;
		input.heldRotate(DEG2RAD * f * rot_dir * dt);
	}

	if (IsKeyPressed(KEY_Z)) {
		if (IsKeyDown(KEY_LEFT_SHIFT))
			world.clearItems();
		else
			world.undoSpawn();
	}
}
