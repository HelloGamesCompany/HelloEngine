#include "BossAttacks.h"
#include "../EbonyMaw/RockDivider.h"
#include "../EbonyMaw/BossLoop.h"
#include <time.h>
#include <random>
#include <math.h>
//Pau Olmos

HELLO_ENGINE_API_C BossAttacks* CreateBossAttacks(ScriptToInspectorInterface* script)
{
	BossAttacks* classInstance = new BossAttacks();
	script->AddDragInt("AttackType", &classInstance->attackType);

	script->AddCheckBox("Attacking", &classInstance->attacking);
	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	script->AddDragBoxGameObject("Player", &classInstance->player);
	script->AddDragBoxGameObject("OrbitingRocks", &classInstance->orbitingRocks);
	script->AddDragFloat("Cooldown", &classInstance->cooldownAttack);
	script->AddDragFloat("RotationSpeed", &classInstance->rotationSpeed);
	script->AddDragFloat("Radius", &classInstance->radius);

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
	script->AddDragBoxGameObject("Rock14", &classInstance->rocks[13]);
	script->AddDragBoxGameObject("Rock15", &classInstance->rocks[14]);
	script->AddDragBoxGameObject("Rock16", &classInstance->rocks[15]);
	script->AddDragBoxGameObject("Rock17", &classInstance->rocks[16]);
	script->AddDragBoxGameObject("Rock18", &classInstance->rocks[17]);
	script->AddDragBoxGameObject("Rock19", &classInstance->rocks[18]);
	script->AddDragBoxGameObject("Rock20", &classInstance->rocks[19]);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void BossAttacks::Start()
{
	for (int i = 0; i < 15; i++) {
		rockPositions[i] = rocks[i].GetTransform().GetGlobalPosition();
	}
	
}

void BossAttacks::Update()
{

	API_GameObject go = boss;;
	BossLoop* bLoop = (BossLoop*)go.GetScript("BossLoop");

	if (bLoop->weakTime > 0) {
		bossState = BOSS_STATE::KO;
		for (int i = 0; i < numRocks[attackType]; i++) {
			ReturnRock(&rocks[currentRock[i]], i, true);
		}
	}
	else if(bossState == BOSS_STATE::KO) {
		bossState = BOSS_STATE::IDLE;
	}
	OrbitingRocks(&rocks[19], &rocks[18], &rocks[17], &rocks[16], rotationSpeed, radius);
	int klk = bLoop->phase;
	switch (klk)
	{
	case 0:
		break;
	case 1:
		OrbitingRocks(&rocks[19], &rocks[18], &rocks[17], &rocks[16], rotationSpeed, radius);
		break;
	case 2:
		OrbitingRocks(&rocks[19], &rocks[18], &rocks[17], &rocks[16], rotationSpeed * 1.5, radius * 0.25);
	default:
		break;
	}


	if (attacking) {
		switch (attackType)
		{
		case 0:
		case 1:
		case 2:
			speed = 1.0f;
			switch (bossState)
			{
			case BossAttacks::BOSS_STATE::ROCKSELECT: SelectRock();
				break;
			case BossAttacks::BOSS_STATE::SEEKING:
				bossPosition[0] = { gameObject.GetTransform().GetGlobalPosition().x, gameObject.GetTransform().GetGlobalPosition().y + 10.0f,gameObject.GetTransform().GetGlobalPosition().z - 4.0f };
				bossPosition[1] = { gameObject.GetTransform().GetGlobalPosition().x + 4.0f,gameObject.GetTransform().GetGlobalPosition().y + 8.0f,gameObject.GetTransform().GetGlobalPosition().z + 4.0f };
				bossPosition[2] = { gameObject.GetTransform().GetGlobalPosition().x - 4.0f,gameObject.GetTransform().GetGlobalPosition().y + 8.0f,gameObject.GetTransform().GetGlobalPosition().z + 4.0f };
				bossPosition[3] = { gameObject.GetTransform().GetGlobalPosition().x + 6.0f,gameObject.GetTransform().GetGlobalPosition().y + 13.0f,gameObject.GetTransform().GetGlobalPosition().z };
				bossPosition[4] = { gameObject.GetTransform().GetGlobalPosition().x - 6.0f,gameObject.GetTransform().GetGlobalPosition().y + 13.0f,gameObject.GetTransform().GetGlobalPosition().z };
				
				for (int i = 0; i < numRocks[attackType]; i++) {
					Seek(&rocks[currentRock[i]], bossPosition[i], speed / 3, i, false);
				}
				break;
			case BossAttacks::BOSS_STATE::HOLDING: HoldRock();
				break;
			case BossAttacks::BOSS_STATE::THROWING:
				currentTimeAttack += Time::GetDeltaTime();

				for (int i = 0; i < numRocks[attackType]; i++) {
					if (currentTimeAttack <= timeAttack[i]) {
						playerPosition[i] = player.GetTransform().GetGlobalPosition();
						dir[i] = player.GetTransform().GetGlobalPosition() - rocks[currentRock[i]].GetTransform().GetGlobalPosition();

					}
					if (currentTimeAttack > timeAttack[i] && hasReachedTarget[i] == false) Seek(&rocks[currentRock[i]], playerPosition[i], speed / 2, i, false);
				}

				if (currentTimeAttack > timeAttack[numRocks[attackType]]) {
					bossState = BOSS_STATE::IDLE;
					attacking = false;
				}

				break;
			default:
				break;
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
			for (int i = 0; i < 5; i++) {
				hasReachedTarget[i] = false;
				playerPosition[i] = { 0,0,0 };
				dir[i] = { 0,0,0 };
			}
			for (int i = 0; i < 15; i++) {
				rocks[i].GetTransform().SetPosition(rockPositions[i]);
			}
			cooldownAttack += Time::GetDeltaTime();
			if (cooldownAttack > 3.0f) {
				bossState = BOSS_STATE::SELECTATTACK;
				cooldownAttack = 0;
			}
			break;
		case BOSS_STATE::SELECTATTACK:
			//srand(time(NULL));
			// Generate a random integer between 0 and 0
			//attackType = rand() % 3;
			attackType = 0;
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
	bossState = BOSS_STATE::SEEKING;

}

void BossAttacks::Seek(API_GameObject* seeker, API_Vector3 target, float speed, int rock, float endedAttacking)
{
	dt = Time::GetDeltaTime();
	if (bossState == BOSS_STATE::SEEKING) {
	dir[rock] = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Rotate(150 * dt, 300 * dt, 250 * dt);
	} 
	API_Vector3 dist = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Translate((dir[rock] / 60.0f) * speed * dt * 400);

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
	for (int i = 0; i < numRocks[attackType]; i++) {
		rocks[currentRock[i]].GetTransform().Rotate(300 * dt, 600 * dt, 500 * dt);
	}
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

void BossAttacks::OrbitingRocks(API_GameObject* orbitingRock1, API_GameObject* orbitingRock2, API_GameObject* orbitingRock3, API_GameObject* orbitingRock4, float rotationSpeed, float radius)
{
	orbitingRock1->GetTransform().SetPosition(radius, 0, 0);
	orbitingRock2->GetTransform().SetPosition(-radius, 0, 0);
	orbitingRock3->GetTransform().SetPosition(0, 0, radius);
	orbitingRock4->GetTransform().SetPosition(0, 0, -radius);
	orbitingRocks.GetTransform().Rotate(0, rotationSpeed, 0);

}
