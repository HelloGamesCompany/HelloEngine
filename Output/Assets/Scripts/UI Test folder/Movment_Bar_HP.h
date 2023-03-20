#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Movment_Bar_HP : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void Speed_HP(bool Low_HP);

	bool Low_HP = false;

	API_Transform Movment_Bar_HP;

	API_Vector3 directionY;
	API_Vector3 directionYfast;

	float timer = 1.0f;
	float timer2 = 0.5f;
};

