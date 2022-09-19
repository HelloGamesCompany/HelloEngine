#include "PhysBody3D.h"
#include "glmath.h"
#include "External/Bullet/include/btBulletDynamicsCommon.h"

PhysBody3D::PhysBody3D(btRigidBody* body) : body(body)
{
}

// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body, GameObject* gameObject) : body(body)
{
	this->gameObject = gameObject;
}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	//btCollisionShape* shape = body->getCollisionShape();
	//btMotionState* motionState = body->getMotionState();
	//RELEASE(shape);
	//RELEASE(motionState);
	//RELEASE(body);
}

// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
const vec3 PhysBody3D::GetPos() const
{
	btTransform t = body->getWorldTransform();
	btVector3 v = t.getOrigin();
	vec3 ret; ret.Set(v.getX(), v.getY(), v.getZ());
	return ret;
}

void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}

const vec3 PhysBody3D::GetLinearVelocity()
{
	const btVector3 v = body->getLinearVelocity();
	vec3 ret = (v.getX(), v.getY(), v.getZ());
	return ret;
}

void PhysBody3D::SetLinearVelocity(float x, float y, float z)
{
	btVector3 v(x, y, z);
	body->setLinearVelocity(v);
}

const vec3 PhysBody3D::GetAngularVelocity()
{
	const btVector3 v = body->getAngularVelocity();
	vec3 ret = (v.getX(), v.getY(), v.getZ());
	return ret;
}

void PhysBody3D::SetAngularVelocity(float x, float y, float z)
{
	btVector3 v(x, y, z);
	body->setAngularVelocity(v);
}
