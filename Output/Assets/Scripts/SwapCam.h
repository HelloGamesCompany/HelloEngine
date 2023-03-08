#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class SwapCam : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_Camera starShipCamera;
	API_Camera mainCamera;
	bool mainCamActive = true;
};

