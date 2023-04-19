#include "ThanosAttacks.h"
HELLO_ENGINE_API_C ThanosAttacks* CreateThanosAttacks(ScriptToInspectorInterface* script)
{
	ThanosAttacks* classInstance = new ThanosAttacks();
	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	script->AddDragBoxGameObject("Player", &classInstance->player);

	script->AddDragBoxGameObject("Melee Attack 1", &classInstance->melee1);

	

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ThanosAttacks::Start()
{
	Tmovement = (ThanosMovement*)boss.GetScript("ThanosMovement");
}
void ThanosAttacks::Update()
{
	if (isAttacking) {
		switch (thanosState)
		{
		case THANOS_STATE::MELEEATTACK:

			MeleeAttack();

			break;

		case THANOS_STATE::DASHATTACK:

			DashAttack();

			break;
		default:
			break;
		}
	}
	
}

void ThanosAttacks::MeleeAttack() {

	meleeAttackTime += Time::GetDeltaTime();
	if (meleeAttackTime >= 0.5f) {
		melee1.SetActive(false);
		meleeAttackTime = 0.0f;
		isAttacking = false;
	}
	else if (meleeAttackTime > 0.05f) {
		melee1.SetActive(true);
	}

}

void ThanosAttacks::DashAttack() {
	Seek(&boss, playerPosition, Tmovement->bossSpeed * 5);
}

void ThanosAttacks::Seek(API_GameObject* seeker, API_Vector3 target, float speed)
{
	API_Vector3 direction = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Translate(direction * speed / 100);

	if (direction.x < 0.03 && direction.x > -0.03 && direction.y < 0.03 && direction.y && direction.z < 0.03 && direction.z) {
		thanosState = THANOS_STATE::MELEEATTACK;
	}
}