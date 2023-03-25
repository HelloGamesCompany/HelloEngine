#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Level2FinalText : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_UIButton continueButton;
};

