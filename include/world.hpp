#pragma once

#include <raylib.h>
#include <vector>
#include <box2d/box2d.h>
#include <variant>
#include "entities/entity.hpp"
#include "entities/spawnable.hpp"
#include "entities/person.hpp"
#include "constants.hpp"

struct GameContext;

constexpr Vector2 BACKGROUND_AREA_SIZE = {
	50 * METER_TO_PIXEL,
	20 * METER_TO_PIXEL
};

struct WorldItem {
	std::variant<Entity, Person> item;

	WorldItem(const std::variant<Entity, Person> &it) : item(it) {}

	template<typename T>
	bool isType() const
	{
		return std::holds_alternative<T>(item);
	}

	template<typename T>
	T *get_if()
	{
		return std::get_if<T>(&item);
	}
	template<typename T>
	const T *get_if() const
	{
		return std::get_if<T>(&item);
	}

	void destroy()
	{
		if (Entity *ent = std::get_if<Entity>(&item))
			ent->rigidbody.destroy();
		else if (Person *per = std::get_if<Person>(&item))
			per->destroy();
	}
};

struct World {
	Rectangle background_area;
	Texture2D background;

	std::vector<WorldItem> items;

	void initialize(b2WorldId world_id);
	void clearItems();
	void undoSpawn();

	void update(GameContext &context);

	void spawnItem(b2WorldId world_id, const Spawnable &,
		       Vector2 position, bool flip = false);
	void spawnPerson(b2WorldId id, Vector2 position,
			  bool flip = false);

	Rigidbody *getBodyAt(Vector2 position);

	std::vector<Entity *> getEntities();
	std::vector<Person *> getPeople();

	/*
	 * return: where the camera target should be at start
	 */
	Vector2 getStartPoint() const
	{
		return {
			GetScreenWidth() * 0.25f,
			getGroundY() - 2 * METER_TO_PIXEL,
		};
	}

	float getGroundY() const
	{
		return background_area.y + background_area.height;
	}

	void drawForeground() const;
	void drawBackground() const;

	private:
	void createWorldBounds(b2WorldId world_id);
};
