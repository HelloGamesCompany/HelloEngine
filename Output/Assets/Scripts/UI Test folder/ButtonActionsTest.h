#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_Vector3.h"
#include "API/API_UIImage.h"

class ButtonActionsTest : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_UIButton apiButton;
	//API_UIImage apiImage;

	API_Vector3 defaultPos;
	bool isHovering;
	bool isGoingUp;
	float minY;
	float maxY;
	float animVel;

	bool hasStarted;
};

