#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API.h"
#include "ThanosMeleeDmg.h"

class ThanosAttacks : HelloBehavior
{
public:
	void Start() override;
	void Update() override;

	void MeleeAttack();

	void DashAttack();

	void Seek(API_GameObject* seeker, API_Vector3 target, float speed);

	ThanosMeleeDmg* tMeleeDmg;
	bool isAttacking = false;

	API_GameObject boss;
	API_GameObject player;

	API_GameObject melee1;
	float meleeAttackTime = 0.0f;

	API_GameObject sword;
	bool swordThrown = false;
	float swordSpeed = 2.0f;
	API_Vector3 aimPosition;

	API_GameObject defenseSword;

	enum class THANOS_STATE {
		IDLE,
		SEEKING,
		MELEEATTACK,
		DASHATTACK,
		HOLDING,
		SPECIALATTACK,
		THROWINGATTACK,
	};

	THANOS_STATE thanosState;

	API_Vector3 playerPosition;
};

