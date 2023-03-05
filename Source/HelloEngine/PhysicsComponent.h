#pragma once
#include "Component.h"
#include "Globals.h"
#include "PhysBody3D.h"
#include "ModulePhysics.h"

class PhysicsComponent : public Component
{
public:

	PhysicsComponent(GameObject* gameObject);

	~PhysicsComponent();

private:
	void OnEditor() override;

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	//Physics
	void CheckShapes();

	void CallUpdatePos();
	void CallUpdateMass();

	void CreateCollider();

	void RemoveCollider();

	void OnTransformCallback(float4x4 worldMatrix);

private:

	PhysBody3D* physBody = nullptr;

	float mass;

	ColliderShape shapeSelected;

	bool isShapeSelected[3];
	bool isShapeCreated[3];
	bool isStatic;

	float3 colPos;
	float3 colRot;
	float3 colScl;

	float sphereRadius;
	float2 cylRadiusHeight;

	bool isRenderingCol;
};
