#pragma once
#include "Component.h"

class CameraComponent : public Component
{
public:
	CameraComponent(GameObject* gameObject);
	~CameraComponent();

	void OnPositionUpdate(float3 pos) override;
	void OnScaleUpdate(float3 scale) override;
	void OnRotationUpdate(float3 rotation) override;
	void OnTransformUpdate(float3 pos, float3 scale, float3 rotation) override;
private:
	ModuleCamera3D* moduleCameras = nullptr;

	uint cameraID = 0;
};

