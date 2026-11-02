#pragma once

#include <box2d/box2d.h>

/*
 * revolute joint
 */
struct JointRev {
	b2JointId id;
	b2RevoluteJointDef def;

	/*
	 * @collide: enable connected collision
	 */
	JointRev(b2WorldId world, b2BodyId a, b2BodyId b, b2Vec2 point,
		 bool collide);

	/*
	 * return: joint rotation limits {x=min, y=max}
	 */
	b2Vec2 getLimits() const
	{
		return {
			b2RevoluteJoint_GetLowerLimit(id),
			b2RevoluteJoint_GetUpperLimit(id)
		};
	}
	float getHertz() const
	{
		return b2RevoluteJoint_GetSpringHertz(id);
	}

	void set_hertz(float f)
	{
		b2RevoluteJoint_SetSpringHertz(id, f);
	}
	void set_damping_ratio(float f)
	{
		b2RevoluteJoint_SetSpringDampingRatio(id, f);
	}
	void enableMotor(bool b)
	{
		b2RevoluteJoint_EnableMotor(id, b);
	}
	/*
	 * set motor speed in radians per second
	 */
	void setMotorSpeed(float f)
	{
		b2RevoluteJoint_SetMotorSpeed(id, f);
	}
	/*
	 * set max motor torque
	 */
	void set_max_torque(float f)
	{
		b2RevoluteJoint_SetMaxMotorTorque(id, f);
	}
	void enable_limit(bool b)
	{
		b2RevoluteJoint_EnableLimit(id, b);
	}
	/*
	 * set joint limit in radians
	 */
	void set_limits(float min, float max)
	{
		b2RevoluteJoint_SetLimits(id, min, max);
	}
	void enableSpring(bool b)
	{
		b2RevoluteJoint_EnableSpring(id, b);
	}
};
