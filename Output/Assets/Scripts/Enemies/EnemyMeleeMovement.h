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

	float DisanceToObj(API_Vector3 obj1, API_Vector3 obj2);

	void Seek();

	bool wander = false;
	bool targeting = false;

	float detectionDis=2.0f;

	API_GameObject point1;
	API_GameObject point2;

	
	

};

