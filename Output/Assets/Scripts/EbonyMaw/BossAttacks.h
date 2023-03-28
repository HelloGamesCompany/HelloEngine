
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
	void Seek(API_GameObject* seeker, API_Transform target, float speed);
	void SelectRock();
	void HoldRock();
	void ReturnRock(API_GameObject* rock);

	API_GameObject rocks[20];
	API_Vector3 rockPositions[20];
	API_GameObject selectedRock;

	API_GameObject boss;
	API_GameObject player;
	API_Transform playerPosition;
	API_Transform bossPosition;
	bool hasPlayerCoords = false;
	bool hasBossCoords = false;

	float xDistance = 0.0f;
	float yDistance = 0.0f;
	float zDistance = 0.0f;

	int attackType = 1;
	int currentRock = -1;
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

	enum class BOSS_STATE {
		IDLE,
		SELECTATTACK,
		ROCKSELECT,
		SEEKING,
		HOLDING,
		THROWING,
	};

	BOSS_STATE bossState = BOSS_STATE::IDLE;

};
