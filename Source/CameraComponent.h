#pragma once
#include "Component.h"
#include "CameraObject.h"

class CameraComponent : public Component
{
public:
	CameraComponent(GameObject* gameObject);
	~CameraComponent();

	void OnTransformCallback(float4x4 worldMatrix) override;

	void OnEditor() override;

	void OnEnable() override;
	void OnDisable() override;

public:
	CameraObject* cameraObject = nullptr;
	bool isCurrentlyDisplaying = false;

private:
	ModuleCamera3D* moduleCameras = nullptr;


};

