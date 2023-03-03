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

	//player velocity
	float vel = 1.0f;
};

