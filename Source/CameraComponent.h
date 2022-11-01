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

	void OnEditor() override;

	void Enable(bool enabled, bool fromGo = true) override;

	CameraObject* GetCameraObject();
public:
	CameraObject* cameraObject = nullptr;

	uint cameraID = 0;
private:
	ModuleCamera3D* moduleCameras = nullptr;


};

