#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerGamepadMovement : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void GamepadAim();

	//player velocity on gamepad
	float vel = 1.0f;

	float maxVel = 0.5f;
	float accel = 0.06f;

	API_Transform cam;

private:
	float _angle;
};

