#include "ThanosAttacks.h"
#include "ThanosMeleeDmg.h"
HELLO_ENGINE_API_C ThanosAttacks* CreateThanosAttacks(ScriptToInspectorInterface* script)
{
	ThanosAttacks* classInstance = new ThanosAttacks();
	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	script->AddDragBoxGameObject("Player", &classInstance->player);

	script->AddDragBoxGameObject("Melee Attack 1", &classInstance->melee1); 
	script->AddDragBoxGameObject("SWORD", &classInstance->sword);
	script->AddDragBoxGameObject("DeflectProjectiles", &classInstance->defenseSword);

	

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ThanosAttacks::Start()
{
	srand(time(NULL));
	tMeleeDmg = (ThanosMeleeDmg*)boss.GetScript("ThanosMeleeDmg");
	thanosState = THANOS_STATE::SEEKING;
	sword.SetActive(false);
	melee1.SetActive(false);
}
void ThanosAttacks::Update()
{
	if (isAttacking) {
		defenseSword.SetActive(false);
		switch (thanosState)
		{
		case THANOS_STATE::IDLE:
		{
			float selectAttack = rand() % 10 + 1;

			if (selectAttack < 6) {
				thanosState = THANOS_STATE::THROWINGATTACK;
				sword.SetActive(true);
			}
			else {

				thanosState = THANOS_STATE::DASHATTACK;
			}


		}break;
		case THANOS_STATE::MELEEATTACK:
			//tMeleeDmg->meleeDmg = 30;

			MeleeAttack();

			break;

		case THANOS_STATE::DASHATTACK:
			//tMeleeDmg->meleeDmg = 50;

			DashAttack();

			break;
		case THANOS_STATE::THROWINGATTACK:
			isAttacking = true;
			if (swordThrown == false) {
				aimPosition = player.GetTransform().GetGlobalPosition();
				swordThrown = true;
			}

			Seek(&sword, aimPosition, swordSpeed);

			break;
		default:
			break;
		}
	}
	else {
		sword.GetTransform().SetPosition(boss.GetTransform().GetGlobalPosition());
		defenseSword.SetActive(true);
	}
	
}

void ThanosAttacks::MeleeAttack() {

	meleeAttackTime += Time::GetDeltaTime();
	if (meleeAttackTime >= 0.5f) {
		melee1.SetActive(false);
		meleeAttackTime = 0.0f;
		isAttacking = false;
		thanosState = THANOS_STATE::SEEKING;
	}
	else if (meleeAttackTime > 0.05f) {
		melee1.SetActive(true);
	}

}

void ThanosAttacks::DashAttack() {
	Seek(&boss, playerPosition, 2.5f);
}

void ThanosAttacks::Seek(API_GameObject* seeker, API_Vector3 target, float speed)
{
	API_Vector3 direction = target - seeker->GetTransform().GetGlobalPosition();
	Console::Log(target);
	seeker->GetTransform().Translate(direction * speed / 10);

	if (direction.x < 0.03 && direction.x > -0.03 && direction.y < 0.03 && direction.y && direction.z < 0.03 && direction.z) {
		if (thanosState == THANOS_STATE::THROWINGATTACK) {
			if (aimPosition == boss.GetTransform().GetGlobalPosition()) {
				swordThrown = false;
				isAttacking = false;
				thanosState = THANOS_STATE::SEEKING;
				sword.SetActive(false);
			}
			aimPosition = boss.GetTransform().GetGlobalPosition();
		}
		else thanosState = THANOS_STATE::MELEEATTACK;
	}
}

