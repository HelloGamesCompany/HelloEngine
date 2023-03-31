#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "../Player/PlayerStats.h"
#include "../EbonyMaw/BossAttacks.h"

class ExplosionWave : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void OnCollisionEnter(API::API_RigidBody other);
	API_GameObject boss;
	PlayerStats* pStats;
	BossAttacks* bAttacks;
	float explosionDmg = 50.0f;
};

