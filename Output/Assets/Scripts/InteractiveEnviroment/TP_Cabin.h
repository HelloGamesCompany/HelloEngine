#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class TP_Cabin : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void OnCollisionEnter(API_RigidBody other);

	API_Transform destination;
	API_ParticleSystem smoke;
	API_GameObject playerRef;


	float timeHoldButton = 0.0f;
	bool playerInTeleport = false;
};

