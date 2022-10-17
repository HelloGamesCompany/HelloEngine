#pragma once
#include "ImWindow.h"

class ModuleCamera3D;

class ImWindowScene : public ImWindow
{
public:
	ImWindowScene();

	~ImWindowScene();

	void Update() override;

private:

	ModuleCamera3D* moduleCamera = nullptr;

	int gameWidth = 1;

	int gameHeight = 1;
};