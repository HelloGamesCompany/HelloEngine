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
	script->AddDragBoxGameObject("Rock3", &classInstance->rocks[2]);
	script->AddDragBoxGameObject("Rock4", &classInstance->rocks[3]);
	script->AddDragBoxGameObject("Rock5", &classInstance->rocks[4]);
	script->AddDragBoxGameObject("Rock6", &classInstance->rocks[5]);
	script->AddDragBoxGameObject("Rock7", &classInstance->rocks[6]);
	script->AddDragBoxGameObject("Rock8", &classInstance->rocks[7]);
	script->AddDragBoxGameObject("Rock9", &classInstance->rocks[8]);
	script->AddDragBoxGameObject("Rock10", &classInstance->rocks[9]);
	script->AddDragBoxGameObject("Rock11", &classInstance->rocks[10]);
	script->AddDragBoxGameObject("Rock12", &classInstance->rocks[11]);
	script->AddDragBoxGameObject("Rock13", &classInstance->rocks[12]);
	script->AddDragBoxGameObject("Rock15", &classInstance->rocks[13]);
	script->AddDragBoxGameObject("Rock15", &classInstance->rocks[14]);
	script->AddDragBoxGameObject("Rock16", &classInstance->rocks[15]);
	script->AddDragBoxGameObject("Rock17", &classInstance->rocks[16]);
	script->AddDragBoxGameObject("Rock18", &classInstance->rocks[17]);
	script->AddDragBoxGameObject("Rock19", &classInstance->rocks[18]);
	script->AddDragBoxGameObject("Rock20", &classInstance->rocks[19]);
	script->AddCheckBox("Attacking", &classInstance->attacking);
	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	script->AddDragBoxGameObject("Player", &classInstance->player);
	script->AddDragFloat("Cooldown", &classInstance->cooldownAttack);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void BossAttacks::Start()
{
	for (int i = 0; i < 19; i++) {
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
			ReturnRock(&rocks[currentRock[0]], 0, true);
			ReturnRock(&rocks[currentRock[1]], 1, true);
			ReturnRock(&rocks[currentRock[2]], 2, true);
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
				bossPosition2 = { gameObject.GetTransform().GetGlobalPosition().x + 4.0f,gameObject.GetTransform().GetGlobalPosition().y +8.0f,gameObject.GetTransform().GetGlobalPosition().z };
				bossPosition3 = { gameObject.GetTransform().GetGlobalPosition().x - 4.0f,gameObject.GetTransform().GetGlobalPosition().y +8.0f,gameObject.GetTransform().GetGlobalPosition().z };
				Seek(&rocks[currentRock[0]], bossPosition1, speed / 3, 0, false);
				Seek(&rocks[currentRock[1]], bossPosition2, speed / 3, 1, false);
				Seek(&rocks[currentRock[2]], bossPosition3, speed / 3, 2, false);
			}
			if (bossState == BOSS_STATE::HOLDING) HoldRock();
			if (bossState == BOSS_STATE::THROWING) {
				currentTimeAttack += Time::GetDeltaTime();
				if (playerPosition[0] == 0) {
					playerPosition[0] = player.GetTransform().GetGlobalPosition();
					dir[0] = player.GetTransform().GetGlobalPosition() - rocks[currentRock[0]].GetTransform().GetGlobalPosition();

				}
				if (currentTimeAttack > timeAttack[0] && hasReachedTarget[0] == false) Seek(&rocks[currentRock[0]], playerPosition[0], speed / 2, 0, false);
				if (playerPosition[1] == 0) {
					dir[1] = player.GetTransform().GetGlobalPosition() - rocks[currentRock[1]].GetTransform().GetGlobalPosition();
					playerPosition[1] = player.GetTransform().GetGlobalPosition();
				}
				if (currentTimeAttack > timeAttack[1] && hasReachedTarget[1] == false) Seek(&rocks[currentRock[1]], playerPosition[1], speed / 2, 1, false);
				if (playerPosition[2] == 0) {
					dir[2] = player.GetTransform().GetGlobalPosition() - rocks[currentRock[2]].GetTransform().GetGlobalPosition();
					playerPosition[2] = player.GetTransform().GetGlobalPosition();
				}
				if (currentTimeAttack > timeAttack[2] && hasReachedTarget[2] == false) Seek(&rocks[currentRock[2]], playerPosition[2], speed / 2, 2, true);
				if (currentTimeAttack > 5.0f) {
					bossState = BOSS_STATE::IDLE;
					attacking = false;
				}
			}
			break;
		default:
			break;
		}
	}
	else {
		switch (bossState)
		{
		case BOSS_STATE::IDLE:
			currentTimeAttack = 0.0f;
			hasReachedTarget[0] = false;
			hasReachedTarget[1] = false;
			hasReachedTarget[2] = false;
			hasReachedTarget[3] = false;
			hasReachedTarget[4] = false;
			for (int i = 0; i < 19; i++) {
				rocks[i].GetTransform().SetPosition(rockPositions[i]);
			}
			playerPosition[0] = { 0,0,0 };
			playerPosition[1] = { 0,0,0 };
			playerPosition[2] = { 0,0,0 };
			playerPosition[3] = { 0,0,0 };
			playerPosition[4] = { 0,0,0 };
			dt = Time::GetDeltaTime();
			cooldownAttack += dt;
			if (cooldownAttack > 5.0f) {
				bossState = BOSS_STATE::SELECTATTACK;
				cooldownAttack = 0;
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
	throwing = false;
	currentRock[0] = 0;
	currentRock[1] = 1;
	currentRock[2] = 2;
	currentRock[3] = 3;
	currentRock[4] = 4;
	if (currentRock[0] > 15) currentRock[0] = 0;
	if (currentRock[1] > 16) currentRock[1] = 0;
	if (currentRock[2] > 17) currentRock[2] = 0;
	if (currentRock[3] > 18) currentRock[3] = 0;
	if (currentRock[4] > 19) currentRock[4] = 0;
	bossState = BOSS_STATE::SEEKING;

}

void BossAttacks::Seek(API_GameObject* seeker, API_Vector3 target, float speed, int rock, float endedAttacking)
{
	if (bossState == BOSS_STATE::SEEKING) {
	dir[rock] = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Rotate(150 * Time::GetDeltaTime(), 300 * Time::GetDeltaTime(), 250 * Time::GetDeltaTime());
	} 
	API_Vector3 dist = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Translate((dir[rock] / 60.0f) * speed * Time::GetDeltaTime() * 400);

	if (dist.x < 0.03 && dist.x > -0.03 && dist.y < 0.03 && dist.y && dist.z < 0.03 && dist.z) {
		if (bossState == BOSS_STATE::SEEKING) {
			bossState = BOSS_STATE::HOLDING;
		}
		if (bossState == BOSS_STATE::THROWING) {
			if(rock == 2) ReturnRock(seeker, rock, true);
			else ReturnRock(seeker, rock, false);
			
		}
	}
}

void BossAttacks::HoldRock()
{
	dt = Time::GetDeltaTime();
	preAttack += dt;
	rocks[currentRock[0]].GetTransform().Rotate(300 * dt, 600 * dt, 500 * dt);
	rocks[currentRock[1]].GetTransform().Rotate(300 * dt, 600 * dt, 500 * dt);
	rocks[currentRock[2]].GetTransform().Rotate(300 * dt, 600 * dt, 500 * dt);
	throwing = true;
	bossState = BOSS_STATE::THROWING;
}

void BossAttacks::ReturnRock(API_GameObject* rock, int numRock, float endedAttacking)
{
	hasReachedTarget[numRock] = true;
	rock->GetTransform().SetPosition(rockPositions[currentRock[numRock]]);
	if (bossState != BOSS_STATE::KO && endedAttacking == true) {
		bossState = BOSS_STATE::IDLE;
	}
	if (endedAttacking == true) {
		attacking = false;
	}
	else {
		attacking = true;
	}
}
