#include "ThanosAttacks.h"
#include "ThanosMeleeDmg.h"
#include "ThanosLoop.h"
#include "../Player/PlayerStats.h"
#include "../Player/PlayerMove.h"

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

	script->AddDragBoxGameObject("ExplosionWave", &classInstance->explosionWave);

	script->AddDragBoxGameObject("Bullet1", &classInstance->bullet1);
	script->AddDragBoxGameObject("Bullet2", &classInstance->bullet2);
	script->AddDragBoxGameObject("Bullet3", &classInstance->bullet3);


	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ThanosAttacks::Start()
{
	srand(time(NULL));
	tMeleeDmg = (ThanosMeleeDmg*)melee1.GetScript("ThanosMeleeDmg");
	tLoop = (ThanosLoop*)boss.GetScript("ThanosLoop");
	pStats = (PlayerStats*)player.GetScript("PlayerStats");
	pMove = (PlayerMove*)player.GetScript("PlayerMove");
	thanosState = THANOS_STATE::SEEKING;
	sword.SetActive(false);
	melee1.SetActive(false);
	defenseSword.SetActive(true);
	explosionWave.SetActive(false);
	bullet1.SetActive(false);
	bullet2.SetActive(false);
	bullet3.SetActive(false);

	bullets[0] = bullet1;
	bullets[1] = bullet2;
	bullets[2] = bullet3;

}
void ThanosAttacks::Update()
{
	
	if (tLoop->phase == 2 && thanosState != THANOS_STATE::THROWINGATTACK && finalSword == true) {
		//2ND phase!!!



		switch (thanosState)
		{
		case THANOS_STATE::PULSE:

			Console::Log("leleleleleleleelelelelele");

			isAttacking = true;
			distSA = player.GetTransform().GetGlobalPosition().Distance(gameObject.GetTransform().GetGlobalPosition());

			explosionTime += Time::GetDeltaTime();
			explosionWave.GetTransform().Scale(20.0f * Time::GetDeltaTime());
			explosionWave.SetActive(true);
			if (explosionTime < 0.5 && distSA < 30.0 && explosionWave1HasArrived == false) {
				//pStats->TakeDamage(50, 0);
				explosionWave1HasArrived = true;
			}
			if (explosionTime >= 0.5 && distSA > 30.0 && distSA < 60.0 && explosionWave2HasArrived == false) {

				//pStats->TakeDamage(1, 0);

				API_Vector3 normalizedvector = boss.GetTransform().GetGlobalPosition() - player.GetTransform().GetGlobalPosition();
				float x = normalizedvector.x * normalizedvector.x;
				float y = 0;
				float z = normalizedvector.z * normalizedvector.z;
				float sum = x + y + z;
				API_Vector3 direction = { normalizedvector.x / sum, 0, normalizedvector.z / sum };
				//pMove->RecieveImpulse(-direction, 0.25f, 50);

				//KnockBack
				explosionWave2HasArrived = true;
			}
			if (explosionTime > 0.6) {
				explosionWave.SetActive(false);
				isAttacking = false;
				thanosState = THANOS_STATE::IDLE;
			}
			break;
		case THANOS_STATE::IDLE:

			attackType = rand() % 100 + 1;

			isAttacking = true;

			if (attackType > 0) thanosState = THANOS_STATE::BURST;

			break;

		case THANOS_STATE::BURST:
			
			busrstTime += Time::GetDeltaTime();

			for (int i = 0; i < 3; i++) {
				if (busrstTime > burstTimes[i]) {
					if (bulletThrown[i] == false) {
						bulletThrown[i] = true;
						playerPositions[i] = player.GetTransform().GetGlobalPosition();
						bullets[i].SetActive(true);
					}
					BulletSeek(&bullets[i], playerPositions[i], bulletSpeed / 10, i);
				}
				else bullets[i].GetTransform().GetGlobalPosition() = boss.GetTransform().GetLocalPosition();
			}
			
			if (busrstTime > burstTimes[3]) {
				thanosState = THANOS_STATE::IDLE;
				isAttacking = false;
				busrstTime = 0.0f;
			}

			break;
		default:
			break;
		}

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
			Console::Log("telodoyyyy");
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
		else if(thanosState != THANOS_STATE::PULSE) thanosState = THANOS_STATE::MELEEATTACK;
	}
}

void ThanosAttacks::BulletSeek(API_GameObject* seeker, API_Vector3 target, float speed, int numBullet)
{
	API_Vector3 direction = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Translate(direction * speed / 10);

	if (direction.x < 0.03 && direction.x > -0.03 && direction.y < 0.03 && direction.y && direction.z < 0.03 && direction.z) {
		seeker->SetActive(false);
	}

}

