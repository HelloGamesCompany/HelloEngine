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
	void DetectImGuizmoInput();

	void DetectClick();

public:
	float cameraSpeed = 10.0f;

private:
	ModuleCamera3D* _moduleCamera = nullptr;

	SceneCameraObject* _sceneCamera = nullptr;

	int _sceneWidth = 1;

	float4x4 _identity = float4x4::identity;

	ImGuizmo::MODE _imMode = ImGuizmo::MODE::WORLD;

	ImGuizmo::OPERATION _imOperation = ImGuizmo::OPERATION::TRANSLATE;

	int _sceneHeight = 1;
};