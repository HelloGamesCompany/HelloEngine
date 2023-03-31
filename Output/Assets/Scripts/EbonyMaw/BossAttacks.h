
#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class BossAttacks : HelloBehavior
{
public:
	void Start() override;
	void Update() override;
	void Seek(API_GameObject* seeker, API_Vector3 target, float speed, int rock, float endedAttacking);
	void SelectRock();
	void HoldRock();
	void ReturnRock(API_GameObject* rock, int numRock, float endedAttacking);

	bool hasReachedTarget[5] = { false,false,false,false,false };

	API_GameObject rocks[20];
	API_Vector3 rockPositions[20];
	API_GameObject selectedRock;

	API_Vector3 dir[5] = { 0,0,0 };

	API_GameObject boss;
	API_GameObject player;
	API_Vector3 playerPosition[5] = { 0,0,0 };
	API_Vector3 bossPosition[5] = { 0,0,0 };


	float timeAttack[6] = { 2.0f,4.0f,4.5f,5.0f,5.5f,6.0f };
	float currentTimeAttack = 0.0f;
	bool endedAttacking = false;

	bool hasPlayerCoords = false;
	bool hasBossCoords = false;

	float xDistance = 0.0f;
	float yDistance = 0.0f;
	float zDistance = 0.0f;

	int attackType = 0;
	int numRocks[3] = { 3,4,5 };

	int currentRock[5] = { 0,1,2,3,4 };

	bool selectRock = false;
	float speed = 0.0f;

	bool seeking = false;
	bool attacking = false;

	float throwing = false;
	float cooldownAttack = 0.0f;
	float preAttack = 0.0f;
	float dt = 0.0f;

	float rockDmg = 20.0f;
	float stoneDmg = 10.0f;

	float stoneSpeed = 1.0f;
	float stoneLifeTime = 0.5f;

	void OrbitingRocks(API_GameObject* orbitingRock1, API_GameObject* orbitingRock2, API_GameObject* orbitingRock3, API_GameObject* orbitingRock4, float rotationSpeed, float radius);
	API_GameObject orbitingRocks;
	float rotationSpeed = 1;
	float radius = 0.5f;

	enum class BOSS_STATE {
		KO,
		IDLE,
		SELECTATTACK,
		ROCKSELECT,
		SEEKING,
		HOLDING,
		THROWING,
	};

	BOSS_STATE bossState = BOSS_STATE::IDLE;

};
