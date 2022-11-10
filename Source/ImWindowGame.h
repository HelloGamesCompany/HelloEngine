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
private:

	int gameWidth = 1;
	int gameHeight = 1;
};

