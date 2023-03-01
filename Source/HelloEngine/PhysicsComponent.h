#pragma once
#include "Component.h"
#include "Globals.h"

class PhysicsComponent : public Component
{
private:

	PhysicsComponent(GameObject* gameObject);
	~PhysicsComponent();
	
	void OnEditor() override;

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	//Physics
	void CheckShapes();

	void CallUpdateShape();

	void CreateCollider();

private:

	PhysBody3D* physBody;

	ColliderShape shapeSelected;
	bool isShapeSelected[3];
	bool isShapeCreated[3];
	bool isStatic;

	float3 colPos = { 0,0,0 };
	float3 colRot = { 0,0,0 };
	float3 colScl = { 1,1,1 };

	float sphereRadius;
	float2 cylRadiusHeight;

};


