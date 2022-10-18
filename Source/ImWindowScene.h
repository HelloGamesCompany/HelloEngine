#pragma once
#include "ImWindow.h"
#include "SceneCameraObject.h"

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

	int sceneHeight = 1;
};