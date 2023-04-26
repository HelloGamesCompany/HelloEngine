#include "ThanosAttacks.h"
#include "ThanosMeleeDmg.h"
#include "ThanosLoop.h"
HELLO_ENGINE_API_C ThanosAttacks* CreateThanosAttacks(ScriptToInspectorInterface* script)
{
	ThanosAttacks* classInstance = new ThanosAttacks();

	script->AddCheckBox("IsAttacking?", &classInstance->isAttacking);

	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	script->AddDragBoxGameObject("Player", &classInstance->player);

	script->AddDragBoxGameObject("Melee Attack 1", &classInstance->melee1); 
	script->AddDragBoxGameObject("SWORD", &classInstance->sword);
	script->AddDragBoxGameObject("DeflectProjectiles", &classInstance->defenseSword);
	script->AddDragBoxGameObject("BoomerangTarget", &classInstance->bTarget);


	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ThanosAttacks::Start()
{
	srand(time(NULL));
	tMeleeDmg = (ThanosMeleeDmg*)melee1.GetScript("ThanosMeleeDmg");
	tLoop = (ThanosLoop*)boss.GetScript("ThanosLoop");
	thanosState = THANOS_STATE::SEEKING;
	sword.SetActive(false);
	melee1.SetActive(false);
	defenseSword.SetActive(true);
}
void ThanosAttacks::Update()
{
	
	if (tLoop->phase == 2 && thanosState != THANOS_STATE::THROWINGATTACK && finalSword == true) {
		//2ND phase!!!
	}
	else {
		if (isAttacking) {
			defenseSword.GetTransform().SetPosition(0, 100000000, 0);
			switch (thanosState)
			{
			case THANOS_STATE::IDLE:
			{
				float selectAttack = rand() % 10 + 1;

				charge += Time::GetDeltaTime();

				if (charge > 0.25f) {
					charge = 0.0f;
					if (selectAttack < 6 || tLoop->phase == 2) {
						playerPosition = player.GetTransform().GetGlobalPosition();

						thanosState = THANOS_STATE::THROWINGATTACK;
						sword.SetActive(true);
					}
					else {
						playerPosition = player.GetTransform().GetGlobalPosition();

						thanosState = THANOS_STATE::DASHATTACK;
					}
				}

			}break;
			case THANOS_STATE::MELEEATTACK:

				if (dashing == false) {
					tMeleeDmg->meleeDmg = 30;
				}
				MeleeAttack();

				break;

			case THANOS_STATE::DASHATTACK:
					tMeleeDmg->meleeDmg = 50;
					dashing = true;
					DashAttack();

				break;
			case THANOS_STATE::THROWINGATTACK:

				isAttacking = true;
				if (swordThrown == false) {

					aimPosition = bTarget.GetTransform().GetGlobalPosition();

					swordThrown = true;
				}

				Seek(&sword, aimPosition, swordSpeed);
				swordTime += Time::GetDeltaTime();

				if (swordTime > 1.75f) {
					swordThrown = false;
					isAttacking = false;
					thanosState = THANOS_STATE::SEEKING;
					sword.SetActive(false);
					swordTime = 0.0f;
				}


				break;
			default:
				break;
			}
		}
		else {
			sword.GetTransform().SetPosition(boss.GetTransform().GetGlobalPosition());
			defenseSword.GetTransform().SetPosition({ 0,1,1 });
			defenseSword.SetActive(true);
		}
	}
	
}

void ThanosAttacks::MeleeAttack() {

	meleeAttackTime += Time::GetDeltaTime();
	if (meleeAttackTime >= 0.5f) {
		melee1.SetActive(false);
		meleeAttackTime = 0.0f;
		isAttacking = false;
		thanosState = THANOS_STATE::SEEKING;
		dashing = false;
	}
	else if (meleeAttackTime > 0.05f) {
		melee1.SetActive(true);
	}

}

void ThanosAttacks::DashAttack() {
	Seek(&boss, playerPosition, 1.5f);
}

void ThanosAttacks::Seek(API_GameObject* seeker, API_Vector3 target, float speed)
{
	API_Vector3 direction = target - seeker->GetTransform().GetGlobalPosition();
	Console::Log(target);
	seeker->GetTransform().Translate(direction * speed / 10);

	if (direction.x < 0.15 && direction.x > -0.15 && direction.y < 0.15 && direction.y && direction.z < 0.15 && direction.z) {
		if (tLoop->phase == 2) {
			thanosState = THANOS_STATE::PULSE;
			isAttacking = false;
			finalSword = true;
			defenseSword.SetActive(false);
		}
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

