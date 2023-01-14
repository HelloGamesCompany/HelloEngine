#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Tank : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

public:
	float movingSpeed = 1.0f;
	API_Transform turret;
};

