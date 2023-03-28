#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class RockDivider : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void OnCollisionEnter(API::API_RigidBody other);
	void OnCollisionEnterSmall(API::API_RigidBody other);


	API_GameObject boss;
	API_GameObject stone1;
	API_GameObject stone2;
	API_GameObject stone3;
	API_GameObject stone4;
	API_GameObject stone5;
	API_GameObject stone6;
	API_GameObject stone7;
	API_GameObject stone8;

	bool rockDivided = false;
	
	float dt;
	float stoneTime = 0.0f;
};

