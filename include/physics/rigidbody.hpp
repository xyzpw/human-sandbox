#pragma once

#include <raylib.h>
#include <box2d/box2d.h>

struct Rigidbody {
	b2BodyDef def;
	b2BodyId id;
	b2ShapeDef shape_def;
	b2ShapeId shape_id;

	/*
	 * @area: size of shape
	 * @density: density in kg/px^3
	 * @flip: flip rotation
	 */
	void initialize(b2WorldId world, Vector2 position,
			Vector2 area, float density, bool flip);
	/*
	 * destroy body id
	 */
	void destroy() { b2DestroyBody(id); }

	b2Vec2 getPosition() const { return b2Body_GetPosition(id); }
	b2Rot getRotation() const { return b2Body_GetRotation(id); }
	float getMass() const { return b2Body_GetMass(id); }

	void setTransform(b2Vec2 vec, b2Rot rot)
	{
		b2Body_SetTransform(id, vec, rot);
	}

	/*
	 * set transform, keep rotation
	 */
	void setPosition(b2Vec2 v)
	{
		setTransform(v, getRotation());
	}

	/*
	 * set transform, keep position
	 */
	void setRotation(b2Rot r)
	{
		setTransform(getPosition(), r);
	}

	/*
	 * set shape filter category bits
	 */
	void setCollisionCategory(uint16_t bits)
	{
		shape_def.filter.categoryBits = bits;
		b2Shape_SetFilter(shape_id, shape_def.filter);
	}
	/*
	 * set shape filter mask bits
	 */
	void setCollisionMask(uint16_t bits)
	{
		shape_def.filter.maskBits = bits;
		b2Shape_SetFilter(shape_id, shape_def.filter);
	}
};
