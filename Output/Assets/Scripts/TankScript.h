#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class TankScript : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	float movingSpeed = 1.0f;
	API_Transform turret;
	API_Transform shootingPoint;
	API_MeshRenderer bulletMesh;
};

