#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API.h"

class Creationtest : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	float creationTestFloat = 5.0f;
	int intTest = 10;
	API_GameObject testGameObject;
};

