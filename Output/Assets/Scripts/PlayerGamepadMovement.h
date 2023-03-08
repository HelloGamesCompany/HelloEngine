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

	//player velocity on gamepad
	float vel = 1.0f;

	float maxVel = 1.0f;
	float accel = 0.1f;

	void GamepadAim();

private:
	float _angle;
};

