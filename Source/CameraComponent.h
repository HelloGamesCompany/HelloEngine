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
	void PerspectiveEditorOptions();
	void OrthographicEditorOptions();

	void OnEnable() override;
	void OnDisable() override;

	void Serialization(json& j) override;

public:
	CameraObject* cameraObject = nullptr;
	bool isCurrentlyDisplaying = false;

private:
	ModuleCamera3D* moduleCameras = nullptr;
	std::string _comboValues[2] = { "Orthographic", "Perspective" };
	uint _selectedCombo = 1;


};

