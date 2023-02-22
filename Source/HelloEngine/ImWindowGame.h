#pragma once
#include "ImWindow.h"

class CameraObject;
class ModuleCamera3D;

class ImWindowGame : public ImWindow
{
public:
	ImWindowGame();
	~ImWindowGame();

	void Update() override;

	int gamePosX = 1;
	int gamePosY = 1;
	int gameWidth = 1;
	int gameHeight = 1;
};

