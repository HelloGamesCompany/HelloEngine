#pragma once

#include "MathGeoLib.h"
#include "btBulletDynamicsCommon.h"

struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D() {};
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	//float4x4 GetColTransform(float4x4 matrix);
	void SetTransform(const float* matrix) const;
	//void SetColTransform(float4x4 matrix);
	void SetPos(float x, float y, float z);

public:
	btRigidBody* body = nullptr;
	float4x4 globalTrans;
	float4x4 transformMat;
};
