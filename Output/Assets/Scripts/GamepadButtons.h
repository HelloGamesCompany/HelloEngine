#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class GamepadButtons : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_GameObject Panel_Pause;
	API_GameObject Panel_Map;
	bool show_map=true;

};

