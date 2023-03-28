#include "BossAttacks.h"
#include "../EbonyMaw/RockDivider.h"

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
	if (attacking) {
		switch (attackType)
		{
		case 1:
			speed = 1.0f;
			if (bossState == BOSS_STATE::ROCKSELECT) SelectRock();
			if (bossState == BOSS_STATE::SEEKING) Seek(&rocks[currentRock], bossPosition, speed);
			if (bossState == BOSS_STATE::HOLDING) HoldRock();
			if (bossState == BOSS_STATE::THROWING) Seek(&rocks[currentRock], playerPosition, speed);
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
	bossPosition = gameObject.GetTransform();

}

void BossAttacks::Seek(API_GameObject* seeker, API_Transform target, float speed) {

	if (seeker->GetTransform().GetGlobalPosition().x > target.GetGlobalPosition().x) {
		xDistance = seeker->GetTransform().GetGlobalPosition().x - target.GetGlobalPosition().x;
		seeker->GetTransform().SetPosition(seeker->GetTransform().GetGlobalPosition().x - (xDistance / 60) * speed, seeker->GetTransform().GetGlobalPosition().y, seeker->GetTransform().GetGlobalPosition().z);
	}
	if (seeker->GetTransform().GetGlobalPosition().x < target.GetGlobalPosition().x) {
		xDistance = target.GetGlobalPosition().x - seeker->GetTransform().GetGlobalPosition().x;
		seeker->GetTransform().SetPosition(seeker->GetTransform().GetGlobalPosition().x + (xDistance / 60) * speed, seeker->GetTransform().GetGlobalPosition().y, seeker->GetTransform().GetGlobalPosition().z);
	}
	if (seeker->GetTransform().GetGlobalPosition().y > target.GetGlobalPosition().y) {
		yDistance = seeker->GetTransform().GetGlobalPosition().y - target.GetGlobalPosition().y;
		seeker->GetTransform().SetPosition(seeker->GetTransform().GetGlobalPosition().x, seeker->GetTransform().GetGlobalPosition().y - (yDistance / 60) * speed, seeker->GetTransform().GetGlobalPosition().z);
	}
	if (seeker->GetTransform().GetGlobalPosition().y < target.GetGlobalPosition().y) {
		yDistance = target.GetGlobalPosition().y - seeker->GetTransform().GetGlobalPosition().y;
		seeker->GetTransform().SetPosition(seeker->GetTransform().GetGlobalPosition().x, seeker->GetTransform().GetGlobalPosition().y + (yDistance / 60) * speed, seeker->GetTransform().GetGlobalPosition().z);
	}
	if (seeker->GetTransform().GetGlobalPosition().z > target.GetGlobalPosition().z) {
		zDistance = seeker->GetTransform().GetGlobalPosition().z - target.GetGlobalPosition().z;
		seeker->GetTransform().SetPosition(seeker->GetTransform().GetGlobalPosition().x, seeker->GetTransform().GetGlobalPosition().y, seeker->GetTransform().GetGlobalPosition().z - (zDistance / 60) * speed);
	}
	if (seeker->GetTransform().GetGlobalPosition().z < target.GetGlobalPosition().z) {
		zDistance = target.GetGlobalPosition().z - seeker->GetTransform().GetGlobalPosition().z;
		seeker->GetTransform().SetPosition(seeker->GetTransform().GetGlobalPosition().x, seeker->GetTransform().GetGlobalPosition().y, seeker->GetTransform().GetGlobalPosition().z + (zDistance / 60) * speed);
	}
	if (xDistance < 0.01 && xDistance > -0.01 && yDistance < 0.01 && yDistance && zDistance < 0.01 && zDistance) {
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
	rocks[currentRock].GetTransform().SetRotation(rocks[currentRock].GetTransform().GetLocalRotation().x + 0.3, rocks[currentRock].GetTransform().GetLocalRotation().y + 0.6, rocks[currentRock].GetTransform().GetLocalRotation().z + 0.5);
	if (preAttack > 1.2) {
		preAttack = 0.0f;
		throwing = true;
		bossState = BOSS_STATE::THROWING;
		playerPosition = player.GetTransform();
	}
}

void BossAttacks::ReturnRock(API_GameObject* rock)
{
	rock->GetTransform().SetPosition(rockPositions[currentRock]);
	bossState = BOSS_STATE::IDLE;
	attacking = false;
}
