#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class EnemyMeleeMovement : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void Seek(float vel, API_Vector3 tarPos);
	void Wander(float vel, API_Vector3 point);

	API_Vector3 NormalizeVec3(float x, float y, float z);

	bool wander = false;
	bool targeting = false;

	float detectionDis=2.0f;
	float lossingDis = 10.0f;

	API_GameObject point1;
	API_GameObject point2;

	API_GameObject target; 
	
	int numPoint = 1;
	
	API_Vector3 actualPoint;

};




