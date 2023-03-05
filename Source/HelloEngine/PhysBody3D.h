#pragma once

#include "MathGeoLib.h"
#include "btBulletDynamicsCommon.h"
#include "ModulePhysics.h"

struct PhysBody3D
{
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void SetVelocity(float x, float y, float z);
	void GetTransform(float* matrix) const;
	//float4x4 GetColTransform(float4x4 matrix);
	void SetTransform(const float* matrix) const;
	//void SetColTransform(float4x4 matrix);
	void SetPos(float x, float y, float z);
	float3 GetPos();
	float3 GetRotation();
	float3 GetVelocity();
	void Update();

	void SetShape(ColliderShape shape);
	void RenderCollider();

public:
	btRigidBody* body = nullptr;
	float4x4 globalTrans;
	float4x4 transformMat;
	bool isRenderingCol;
	uint gameObjectUID = 0;

	float3 colPos;
	float3 colRot;
	float3 colScl;

	ColliderShape colShape;
};
