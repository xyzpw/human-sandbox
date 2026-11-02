#include "physics/joint.hpp"
#include <box2d/box2d.h>

JointRev::JointRev(b2WorldId world, b2BodyId a, b2BodyId b, b2Vec2 point,
		   bool collide)
{
	def = b2DefaultRevoluteJointDef();
	def.bodyIdA = a;
	def.bodyIdB = b;
	def.localAnchorA = b2Body_GetLocalPoint(a, point);
	def.localAnchorB = b2Body_GetLocalPoint(b, point);
	def.collideConnected = collide;

	id = b2CreateRevoluteJoint(world, &def);
}
