#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PauseButtons : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_UIButton Continue;
	API_UIButton Settings;
	API_UIButton MainMenu;
	API_GameObject Panel;
};

