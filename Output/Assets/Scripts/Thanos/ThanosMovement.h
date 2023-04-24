#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "ThanosAttacks.h"
#include "API/API.h"
class ThanosMovement : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;


	float Rotate(API_Vector3 target, float _angle);
	void Seek(API_GameObject* seeker, API_Vector3 target, float speed);

	API_GameObject player;
	API_GameObject boss;

	float distBP;
	float bossSpeed = 1.0f;

	float angle = 0.0f;

	float inRange = false;
	float dashInRange = false;
	float dashCooldown = 0.0f;

	ThanosAttacks* Tattack;
};

