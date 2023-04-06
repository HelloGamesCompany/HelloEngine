#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "Enemy.h"

class EnemyTank : HelloBehavior
{
public:

	enum class States {
		TARGETING,
		ATTACKIG
	};

	void Start() override;
	void Update() override;

	float TakeDamageTank(float life, float damage);

	/*void Seek(float vel, API_Vector3 tarPos, API_RigidBody rb);
	void Attacking(float vel, API_Vector3 tarPos, API_RigidBody rb);*/

	float currentShield;
	float maxShield;
	float recoverShieldTime;

	float healthRestorePerSecond;
	float healthRestoreCooldown;

	//API_GameObject tank;
	Enemy* enemyScript;
private:
	bool isRecoveringShield;
	bool isRestoringHealth;
	bool hasToRestoreHealth;
	float shieldRecoverCounter;
	float healthRestoreCounter;
};

