#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Movment_Bar : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_Transform Movment_Bar;

	API_Vector3 directionY;

	float timer = 1.0f;
};

