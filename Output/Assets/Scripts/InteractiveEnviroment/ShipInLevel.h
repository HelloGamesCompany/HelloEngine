#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class FadeToBlack;

class ShipInLevel : HelloBehavior
{
public:

	void Start() override; 
	void Update() override;

	void OnCollisionStay(API::API_RigidBody other);

	API_GameObject fadeToBlackGO;
	FadeToBlack* fadeToBlackRef;

	float timerTotp = 0.0f;

};

