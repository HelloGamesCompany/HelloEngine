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

	void SetVelocity(float3 vec);
	float3 GetVelocity();

private:
	void OnEditor() override;

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	//Physics
	void CheckShapes();

	void CallUpdatePos();
	void CallUpdateRotation();
	void CallUpdateScale();
	void CallUpdateMass();

	void CreateCollider();

	void RemoveCollider();

	void OnTransformCallback(float4x4 worldMatrix);

private:

	PhysBody3D* physBody = nullptr;

	ColliderShape shapeSelected;

	bool isShapeSelected[3];
	bool isShapeCreated[3];
	bool isStatic;

public:
	float sphereRadius;
	float2 cylRadiusHeight;
	float renderColColor[4];
	float wireframeSize;

	int sphereHorSlices;
	int sphereVerSlices;

	float temporalMass = 1;
	int cylinderVerSlices;
};
