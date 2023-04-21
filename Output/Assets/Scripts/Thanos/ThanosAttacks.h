#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "../Thanos/ThanosMovement.h"
#include "API/API.h"

class ThanosAttacks : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void MeleeAttack();

	void DashAttack();

	void Seek(API_GameObject* seeker, API_Vector3 target, float speed);

	ThanosMovement* Tmovement;
	bool isAttacking = false;

	API_GameObject boss;
	API_GameObject player;

	API_GameObject melee1;
	float meleeAttackTime = 0.0f;

	enum class THANOS_STATE {
		IDLE,
		SEEKING,
		MELEEATTACK,
		DASHATTACK,
		HOLDING,
		SPECIALATTACK,
	};

	THANOS_STATE thanosState;

	API_Vector3 playerPosition;
};

