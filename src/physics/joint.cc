#include "physics/joint.hpp"
#include <box2d/box2d.h>

JointRev::JointRev(b2WorldId world, b2BodyId a, b2BodyId b, b2Vec2 point,
		   bool collide)
{
	def = b2DefaultRevoluteJointDef();
	def.base.bodyIdA = a;
	def.base.bodyIdB = b;
	def.base.localFrameA.p = b2Body_GetLocalPoint(a, point);
	def.base.localFrameB.p = b2Body_GetLocalPoint(b, point);
	def.base.collideConnected = collide;

	id = b2CreateRevoluteJoint(world, &def);
}
