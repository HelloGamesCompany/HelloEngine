#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "BossAttacks.h"


#include "API/API.h"

class FireDamage : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void OnCollisionEnter(API::API_RigidBody other);

	API_GameObject boss;
	BossAttacks* bAttacks;

};

