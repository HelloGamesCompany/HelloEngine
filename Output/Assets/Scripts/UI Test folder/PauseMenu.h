#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include"API/API_UIImage.h"

class PauseMenu : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_UIImage mapImage;
};

