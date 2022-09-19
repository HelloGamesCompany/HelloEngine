#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "List.h"
#include "glmath.h"

class GameObject;
class btRigidBody;
class Module;
class vec3;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;

public:
	PhysBody3D(btRigidBody* body);
	PhysBody3D(btRigidBody* body, GameObject* gameObject);
	~PhysBody3D();

	void Push(float x, float y, float z);

	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;

	const vec3 GetPos() const;
	void SetPos(float x, float y, float z);

	const vec3 GetLinearVelocity();
	void SetLinearVelocity(float x, float y, float z);

	const vec3 GetAngularVelocity();
	void SetAngularVelocity(float x, float y, float z);

public:
	btRigidBody* body = nullptr;

public:
	GameObject* gameObject = nullptr;
};

#endif // __PhysBody3D_H__