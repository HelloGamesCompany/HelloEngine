#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "../Enemies/EnemyDropManager.h"

#include "API/API.h"

class BreakableBox : HelloBehavior
{
public:

	void Start() override; 
	void Update() override;

	void OnCollisionEnter(API_RigidBody other);

	void ShootBox(float projectileDamage);
	void DestroyBox();
	void CreatePowerUps();

	API_ParticleSystem boxDestroyed;

	int maxHp;
	int currentHp;

	uint meshAmmo;
	uint meshAidKit;
	uint materialAmmo;
	uint materialAidKit;

	API_GameObject enemyDropManagerGO;
	EnemyDropManager* enemyDropManager;

};

