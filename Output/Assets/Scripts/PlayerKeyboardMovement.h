#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerKeyboardMovement : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void MouseAim();

	//player velocity on keyboard
	float velk = 1.0f;

	float maxVel = 1.0f;
	float accel = 0.1f;
	API_Transform cam;

private:
	float _angle;

};

