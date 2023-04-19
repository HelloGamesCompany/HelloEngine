#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class MenuButtons : HelloBehavior
{
public:
	void Start() override;
	void Update() override;
	API_UIButton Continue;
	API_UIButton NewGame;
	API_UIButton Exit;
	API_UIButton Collectibles;
	API_UIButton Settings;
};
