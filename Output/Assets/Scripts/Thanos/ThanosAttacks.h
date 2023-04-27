#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API.h"
#include "ThanosMeleeDmg.h"
#include "ThanosLoop.h"
#include "../Player/PlayerStats.h"
#include "../Player/PlayerMove.h"

class ThanosAttacks : HelloBehavior
{
public:
	void Start() override;
	void Update() override;

	void MeleeAttack();

	void DashAttack();

	void Seek(API_GameObject* seeker, API_Vector3 target, float speed);
	void BulletSeek(API_GameObject* seeker, API_Vector3 target, float speed, int numBullet);

	ThanosMeleeDmg* tMeleeDmg;
	bool isAttacking = false;
	bool dashing = false;

	API_GameObject boss;
	API_GameObject player;

	API_GameObject melee1;
	float meleeAttackTime = 0.0f;

	API_GameObject sword;
	API_GameObject bTarget;
	bool swordThrown = false;
	float swordSpeed = 1.0f;
	API_Vector3 aimPosition;
	float swordTime = 0.0f;

	float charge = 0.0f;

	API_GameObject defenseSword;

	float distSA = 0.0F;

	enum class THANOS_STATE {
		IDLE,
		SEEKING,
		MELEEATTACK,
		DASHATTACK,
		HOLDING,
		SPECIALATTACK,
		THROWINGATTACK,
		LASTSWORD,
		PULSE,
		BURST,
	};

	THANOS_STATE thanosState;

	API_Vector3 playerPosition;

	ThanosLoop* tLoop;
	PlayerStats* pStats;
	PlayerMove* pMove;
	bool finalSword = false;

	float explosionTime = 0.0f;
	API_GameObject explosionWave;

	bool explosionWave1HasArrived = false;
	bool explosionWave2HasArrived = false;


	API_GameObject bullet1;
	API_GameObject bullet2;
	API_GameObject bullet3;

	API_GameObject bullets[3];
	float bulletSpeed = 1.0f;

	bool bulletThrown[3] = { false,false,false };

	API_Vector3 playerPositions[3] = { 0,0,0 };
	float burstTimes[4] = { 0.01f,0.25f,0.5f,1.0f };
	float busrstTime = 0.0f;

	float attackType = 0.0f;
};

