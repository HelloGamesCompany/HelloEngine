#pragma once
#include "ImWindow.h"

class ModuleCamera3D;
class CameraObject;

class ImWindowScene : public ImWindow
{
public:
	ImWindowScene();

	~ImWindowScene();

	void Update() override;

private:

	ModuleCamera3D* moduleCamera = nullptr;

	CameraObject* sceneCamera = nullptr;

	int sceneWidth = 1;

	int sceneHeight = 1;
};