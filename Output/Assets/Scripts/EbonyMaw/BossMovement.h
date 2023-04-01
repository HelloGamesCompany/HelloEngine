#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "BossAttacks.h"
#include "API/API.h"

//#include 

class BossMovement : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	float Rotate(API_Vector3 target, float _angle);
	API_GameObject player;
	//BossAttacks* bAttacks;
	API_GameObject boss;
	float angle = 0.0f;
};

