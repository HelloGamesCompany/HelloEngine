#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class StarShipCamMov : HelloBehavior
{
public:
	float Lerp(float a, float b, float t);
	void Start() override;
	void Update() override;

	API_GameObject target;

	API_Vector3 camPos;
	API_Vector3 camRot;

	API_Vector3 desiredPosition;
	API_Vector3 smoothedPosition;

	float delay = 0.01f;
};

