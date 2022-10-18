#pragma once
#include "Component.h"
#include "CameraObject.h"

class CameraComponent : public Component
{
public:
	CameraComponent(GameObject* gameObject);
	~CameraComponent();

	void OnPositionUpdate(float3 pos) override;
	void OnRotationUpdate(float3 rotation) override;
	void OnTransformUpdate(float3 pos, float3 scale, float3 rotation) override;

	CameraObject* GetCameraObject();
private:
	ModuleCamera3D* moduleCameras = nullptr;

	uint cameraID = 0;
};

