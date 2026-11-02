#include "world.hpp"
#include <raylib.h>
#include <raymath.h>
#include "assets.hpp"
#include "toast.hpp"
#include "entities/spawnable.hpp"
#include "entities/person.hpp"
#include "utils/math.hpp"
#include "utils/display.hpp"
#include "utils/physics.hpp"
#include "utils/raylib.hpp"
#include "physics/rigidbody.hpp"
#include "context.hpp"
#include "constants.hpp"

static float spawnableDensity(SpawnableType t)
{
	float density = 0;
	switch (t) {
	case SpawnableType::person: density = PERSON_DENSITY; break;
	case SpawnableType::cube_metal: density = 2700; break;
	default: break;
	}
	return density * METER_TO_PIXEL;
}

void World::initialize(b2WorldId world_id)
{
	background = asset_manager.getTexture(WORLD_BACKGROUND_CELL_NAME);
	SetTextureWrap(background, TEXTURE_WRAP_REPEAT);

	background_area = {
		0, 0,
		BACKGROUND_AREA_SIZE.x, BACKGROUND_AREA_SIZE.y
	};

	createWorldBounds(world_id);
}

void World::clearItems()
{
	int size = items.size();
	for (int i = 0; i < size; i++) {
		items[i].destroy();
	}
	items.clear();

	toast_manager.notify("cleared items");
}

void World::undoSpawn()
{
	if (items.empty())
		return;

	items.back().destroy();
	items.pop_back();

	toast_manager.notify("undo spawn");
}

void World::update(GameContext &context)
{
	for (auto *p : getPeople()) {
		p->update(context);
	}
}

void World::spawnItem(b2WorldId world_id, const Spawnable &s,
		      Vector2 position, bool flip)
{
	if (s.type == SpawnableType::person) {
		spawnPerson(world_id, position, flip);
		return;
	}

	Entity ent = {
		.texture = asset_manager.getTexture(s.texture_name),
		.tint = s.tint,
	};
	ent.rigidbody.initialize(world_id, position,
				 textureSize(ent.texture) * PIXEL_TO_METER,
				 spawnableDensity(s.type), flip);
	items.emplace_back(ent);

	toast_manager.notify("spawn {}", s.name);
}

void World::spawnPerson(b2WorldId id, Vector2 position, bool flip)
{

	Person person{};
	person.create(id, position, flip);
	person.setPose(PoseType::standing);
	items.emplace_back(person);

	toast_manager.notify("spawn person");
}

Rigidbody *World::getBodyAt(Vector2 position)
{
	b2Vec2 vec = vecToB2(position * PIXEL_TO_METER);
	Rigidbody *result = nullptr;

	/*
	 * find body at position and set result
	 *
	 * return: true on success
	 */
	auto fn = [&](Entity &e) {
		Rigidbody &rb = e.rigidbody;
		if (b2Body_GetShapeCount(rb.id) == 0)
			return false;

		b2ShapeId shapes[8];
		int n = b2Body_GetShapes(rb.id, shapes, 8);
		for (int i = 0; i < n; ++i) {
			if (b2Shape_TestPoint(shapes[i], vec)) {
				result = &rb;
				return true;
			}
		}
		return false;
	};

	for (auto &it : getEntities()) {
		if (fn(*it))
			return result;
	}
	return nullptr;
}

std::vector<Entity *> World::getEntities()
{
	std::vector<Entity *> result;
	for (auto &it : items) {
		if (Person *p = it.get_if<Person>()) {
			for (auto &bp : p->body_parts) {
				result.push_back(&bp);
			}
		}
		if (Entity *ent = it.get_if<Entity>())
			result.push_back(ent);
	}
	return result;
}

std::vector<Person *> World::getPeople()
{
	std::vector<Person *> result;
	for (auto &it : items) {
		Person *p = it.get_if<Person>();
		if (p)
			result.push_back(p);
	}
	return result;
}

void World::drawForeground() const
{
	auto fn = [&](const Entity &e) {
		const Rigidbody &rb = e.rigidbody;
		float rot = -bodyAngle(rb.id) * RAD2DEG;
		Vector2 pos = b2ToVec(rb.getPosition() * METER_TO_PIXEL);
		drawTextureCenter(e.texture, pos, rot,
				  e.flip, e.tint);
	};

	for (const auto &it : items) {
		if (const Person *p = it.get_if<Person>()) {
			for (const auto &bp : p->body_parts) {
				fn(bp);
			}
			continue;
		}
		fn(*it.get_if<Entity>());
	}
}

void World::drawBackground() const
{
	Rectangle src{
		0.f, 0.f,
		background_area.width,
		background_area.height
	};

	DrawTexturePro(background, src, background_area, Vector2Zeros, 0, WHITE);
}

void World::createWorldBounds(b2WorldId world_id)
{
	const static float thickness = 100;
	float x = background_area.x * PIXEL_TO_METER;
	float y = background_area.y * PIXEL_TO_METER;
	float width = background_area.width * PIXEL_TO_METER;
	float height = background_area.height * PIXEL_TO_METER;

	auto fn_wall = [&](b2Vec2 p, float thick_x, float thick_y) {
		b2BodyDef bdef = b2DefaultBodyDef();
		bdef.type = b2_staticBody;
		bdef.position = p;

		b2BodyId bid = b2CreateBody(world_id, &bdef);
		b2Polygon box = b2MakeBox(0.5f * thick_x, 0.5f * thick_y);
		b2ShapeDef shape = b2DefaultShapeDef();
		shape.filter.categoryBits = COLLISION_ALL;
		shape.material.friction = 0.5f;
		shape.enableContactEvents = true;
		shape.enableHitEvents = true;
		b2CreatePolygonShape(bid, &shape, &box);
	};

	b2Vec2 points[4] = {
		{x + width * 0.5f, y + thickness * 0.5f},          // top
		{x + width * 0.5f, y - height - thickness * 0.5f}, // bottom
		{x + width + thickness * 0.5f, y - height * 0.5f}, // right
		{x - thickness * 0.5f, y - height * 0.5f}          // left
	};

	fn_wall(points[0], width, thickness);
	fn_wall(points[1], width, thickness);
	fn_wall(points[2], thickness, width);
	fn_wall(points[3], thickness, width);
}
