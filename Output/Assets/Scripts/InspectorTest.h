#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class InspectorTest : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
public:
	float testFloat = 10.0f;
	int testInt = 3;
	bool testBool = false;
	API_GameObject testGameObject;
};

