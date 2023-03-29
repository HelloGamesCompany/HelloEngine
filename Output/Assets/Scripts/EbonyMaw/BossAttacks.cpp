#include "BossAttacks.h"
#include "../EbonyMaw/RockDivider.h"
#include "../EbonyMaw/BossLoop.h"
#include <time.h>
#include <random>
//Pau Olmos

HELLO_ENGINE_API_C BossAttacks* CreateBossAttacks(ScriptToInspectorInterface* script)
{
	BossAttacks* classInstance = new BossAttacks();
	script->AddDragBoxGameObject("Rock1", &classInstance->rocks[0]);
	script->AddDragBoxGameObject("Rock2", &classInstance->rocks[1]);
	script->AddCheckBox("Attacking", &classInstance->attacking);
	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	script->AddDragBoxGameObject("Player", &classInstance->player);
	script->AddDragFloat("Cooldown", &classInstance->cooldownAttack);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void BossAttacks::Start()
{
	for (int i = 0; i < 2; i++) {
		rockPositions[i] = rocks[i].GetTransform().GetGlobalPosition();
	}
}

void BossAttacks::Update()
{
	API_GameObject go = boss;;
	BossLoop* bLoop = (BossLoop*)go.GetScript("BossLoop");

	if (bLoop->weakTime > 0) {
		bossState = BOSS_STATE::KO;
		switch (attackType)
		{
		case 1:
			ReturnRock(&rocks[currentRock]);
			break;
		default:
			break;
		}
	}
	else if(bossState == BOSS_STATE::KO) {
		bossState = BOSS_STATE::IDLE;
	}

	if (attacking) {
		switch (attackType)
		{
		case 1:
			speed = 1.0f;
			if (bossState == BOSS_STATE::ROCKSELECT) SelectRock();
			if (bossState == BOSS_STATE::SEEKING) {
				bossPosition1 = { gameObject.GetTransform().GetGlobalPosition().x,gameObject.GetTransform().GetGlobalPosition().y +10.0f,gameObject.GetTransform().GetGlobalPosition().z };
				Seek(&rocks[currentRock], bossPosition1, speed / 3);
			}
			if (bossState == BOSS_STATE::HOLDING) HoldRock();
			if (bossState == BOSS_STATE::THROWING) Seek(&rocks[currentRock], playerPosition, speed/2);
			break;
		default:
			break;
		}
	}
	else {
		switch (bossState)
		{
		case BOSS_STATE::IDLE:
			dt = Time::GetDeltaTime();
			cooldownAttack += dt;
			if (cooldownAttack > 5) {
				bossState = BOSS_STATE::SELECTATTACK;
				cooldownAttack = 0;
				dt = 0;
			}
			break;
		case BOSS_STATE::SELECTATTACK:
			srand(time(NULL));
			// Generate a random integer between 0 and 0
			//attackType = rand() % 0 + 1;
			attackType = 1;
			bossState = BOSS_STATE::ROCKSELECT;
			hasBossCoords = false;
			attacking = true;
			break;
		default:
			break;
		}
	}
}

void BossAttacks::SelectRock()
{
	//throwing = false;
	currentRock++;
	if (currentRock > 1) currentRock = 0;
	bossState = BOSS_STATE::SEEKING;

}

void BossAttacks::Seek(API_GameObject* seeker, API_Vector3 target, float speed)
{
	if (bossState == BOSS_STATE::SEEKING) {
	dir = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Rotate(150 * Time::GetDeltaTime(), 300 * Time::GetDeltaTime(), 250 * Time::GetDeltaTime());
	} 
	API_Vector3 dist = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Translate((dir / 60.0f) * speed * Time::GetDeltaTime() * 400);

	if (dist.x < 0.03 && dist.x > -0.03 && dist.y < 0.03 && dist.y && dist.z < 0.03 && dist.z) {
		if (bossState == BOSS_STATE::SEEKING) {
			bossState = BOSS_STATE::HOLDING;
		}
		if (bossState == BOSS_STATE::THROWING) {
			ReturnRock(seeker);
		}
	}
}

void BossAttacks::HoldRock()
{
	dt = Time::GetDeltaTime();
	preAttack += dt;
	rocks[currentRock].GetTransform().Rotate(300 * dt, 600 * dt, 500 * dt);
	if (preAttack > 1.2) {
		preAttack = 0.0f;
		throwing = true;
		bossState = BOSS_STATE::THROWING;
		playerPosition = player.GetTransform().GetGlobalPosition();
		dir = player.GetTransform().GetGlobalPosition() - rocks[currentRock].GetTransform().GetGlobalPosition();
	}
}

void BossAttacks::ReturnRock(API_GameObject* rock)
{
	rock->GetTransform().SetPosition(rockPositions[currentRock]);
	if (bossState != BOSS_STATE::KO) {
		bossState = BOSS_STATE::IDLE;
	}
	attacking = false;
}
