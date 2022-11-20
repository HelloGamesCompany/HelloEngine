#pragma once
#include "ImWindow.h"
#include "SceneCameraObject.h"
#include "ImGuizmo/ImGuizmo.h"

class ModuleCamera3D;

class ImWindowScene : public ImWindow
{
public:
	ImWindowScene();

	~ImWindowScene();

	void Update() override;

private:

	ModuleCamera3D* moduleCamera = nullptr;

	SceneCameraObject* sceneCamera = nullptr;

	int sceneWidth = 1;
	float4x4 identity = float4x4::identity;

	int sceneHeight = 1;
};