#include "EnemyTank.h"
HELLO_ENGINE_API_C EnemyTank* CreateEnemyTank(ScriptToInspectorInterface* script)
{
	EnemyTank* classInstance = new EnemyTank();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("Max Shield", &classInstance->maxShield);
	script->AddDragFloat("Current Shield", &classInstance->currentShield);
	script->AddDragFloat("Recover Shield Time", &classInstance->recoverShieldTime);
	script->AddDragFloat("Hp/s", &classInstance->healthRestorePerSecond);
	script->AddDragFloat("Hp/s Cooldown", &classInstance->healthRestoreCooldown);

	script->AddDragInt("Gun Type(0Semi/1Burst/2Shotgun)", &classInstance->gunType);
	script->AddDragBoxGameObject("Enemy gun", &classInstance->gunObj);

	return classInstance;
}

void EnemyTank::Start()
{
	currentShield = maxShield;
	isRecoveringShield = false;
	isRestoringHealth = false;
	shieldRecoverCounter = 0;

	enemyScript = (Enemy*)gameObject.GetScript("Enemy");
	if (enemyScript->hasShield == false) {
		enemyScript->hasShield = true;
	}

	switch (gunType)
	{
	case 0:
		enemyGun = (EnemyGun*)gunObj.GetScript("EnemyAutomatic");
		break;
	case 1:
		enemyGun = (EnemyGun*)gunObj.GetScript("EnemyBurst");
		break;
	case 2:
		enemyGun = (EnemyGun*)gunObj.GetScript("EnemyShotgun");
		break;
	default:
		break;
	}

}
void EnemyTank::Update()
{
	//Shield
	if (currentShield <= 0) {
		if (isRecoveringShield == false) {
			isRecoveringShield = true;
		}
	}

	if (isRecoveringShield == true) {
		shieldRecoverCounter += Time::GetDeltaTime();
		if (shieldRecoverCounter >= recoverShieldTime) {
			currentShield = maxShield;
			isRecoveringShield = false;
			shieldRecoverCounter = 0;
		}
	}

	//Health
	if (enemyScript->currentHp < enemyScript->maxHp) {
		if (hasToRestoreHealth == false) {
			hasToRestoreHealth = true;
		}
	}

	if (hasToRestoreHealth == true) {
		healthRestoreCounter += Time::GetDeltaTime();
		if (healthRestoreCounter >= healthRestoreCooldown) {
			if (isRestoringHealth == false) {
				isRestoringHealth = true;
			}
		}
	}

	if (isRestoringHealth == true) {

		if (enemyScript->currentHp + Time::GetDeltaTime() * healthRestorePerSecond < enemyScript->maxHp) {
			enemyScript->currentHp += Time::GetDeltaTime() * healthRestorePerSecond;
		}
		else {
			enemyScript->currentHp = enemyScript->maxHp;
			hasToRestoreHealth = false;
			isRestoringHealth = false;
			healthRestoreCounter = 0;
		}
	}

	if (enemyGun != nullptr)
	{
		enemyGun->Shoot();
	}

}

float EnemyTank::TakeDamageTank(float life, float damage)
{
	float prevShield = currentShield;

	currentShield -= damage;

	if (currentShield < 0) {

		life -= std::abs(prevShield - damage);
		currentShield = 0;
		hasToRestoreHealth = false;
		isRestoringHealth = false;
		healthRestoreCounter = 0;
	}


	return life;
}