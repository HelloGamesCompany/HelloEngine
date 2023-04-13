#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class HUBTextManager : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_GameObject fromLevel1Panel;
	API_GameObject fromLevel2Panel;
};

