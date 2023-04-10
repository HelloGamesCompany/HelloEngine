#pragma once
#include "Component.h"
#include "Globals.h"
#include "PhysBody3D.h"
#include "ModulePhysics.h"
#include "API/API_GameObject.h"

class PhysicsComponent : public Component
{
public:

	PhysicsComponent(GameObject* gameObject);

	~PhysicsComponent();

	void SetVelocity(float3 vec);
	float3 GetVelocity();

	void SetGravity(float3 grav);
	float3 GetGravity();
	void CheckRenderBuffers();

	float GetRadius();

	void SetAngularFactor(float3 angle);

private:
#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;
	//Physics
	void CheckShapes();

	void CallUpdatePos();
	void CallUpdateRotation();
	void CallUpdateScale();
	void CallUpdateMass();

	void CallUpdateStatic();
	void CallUpdateTrigger();
	void CallUpdateKinematic();
	void CallUpdateColliderType();

	void CreateCollider();

	void RemoveCollider();

	void OnTransformCallback(float4x4 worldMatrix);
	
	void OnEnable();
	void OnDisable();

private:

	PhysBody3D* _physBody = nullptr;

	ColliderShape _shapeSelected;

	bool _isShapeSelected[3];
	bool _isShapeCreated[3];
	bool _isStatic = false;
	float _gravity[3];
	float _angularFactor[3] = { 1.0f, 1.0f, 1.0f };
	
	friend class API::API_GameObject;
	friend class API::API_RigidBody;

public:
	float localGlobalGravity[3];
	float globalGravity[3];

	float sphereRadius = 0;
	float2 cylRadiusHeight;
	float renderColColor[4];
	float wireframeSize = 0;

	int sphereHorSlices = 0;
	int sphereVerSlices = 0;

	float temporalMass = 1;
	int cylinderVerSlices = 0;
};
