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
	/// <summary>
	/// This reads necessary Input for the ImGuizmo Manipulate.
	/// </summary>
	void DetectSceneInput();

private:

	ModuleCamera3D* moduleCamera = nullptr;

	SceneCameraObject* sceneCamera = nullptr;

	int sceneWidth = 1;
	float4x4 identity = float4x4::identity;

	ImGuizmo::MODE _imMode = ImGuizmo::MODE::WORLD;
	ImGuizmo::OPERATION _imOperation = ImGuizmo::OPERATION::TRANSLATE;

	int sceneHeight = 1;
};