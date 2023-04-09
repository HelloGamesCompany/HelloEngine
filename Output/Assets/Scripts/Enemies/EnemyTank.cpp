#include "EnemyTank.h"
HELLO_ENGINE_API_C EnemyTank* CreateEnemyTank(ScriptToInspectorInterface* script)
{
	EnemyTank* classInstance = new EnemyTank();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragFloat("init X", &classInstance->initialPosition.x);
	script->AddDragFloat("init Y", &classInstance->initialPosition.y);
	script->AddDragFloat("init Z", &classInstance->initialPosition.z);

	script->AddDragFloat("Init Pos Range", &classInstance->initPosRange);

	script->AddDragFloat("Max Shield", &classInstance->maxShield);
	script->AddDragFloat("Current Shield", &classInstance->currentShield);
	script->AddDragFloat("Recover Shield Time", &classInstance->recoverShieldTime);
	script->AddDragFloat("Hp/s", &classInstance->healthRestorePerSecond);
	script->AddDragFloat("Hp/s Cooldown", &classInstance->healthRestoreCooldown);

	script->AddDragFloat("Max Shield", &classInstance->maxShield);

	script->AddDragFloat("Detection Distance", &classInstance->detectionDistance);
	script->AddDragFloat("Walk Velocity", &classInstance->walkVelocity);
	script->AddDragFloat("Seek Velocity", &classInstance->seekVelocity);

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

	initialPosition = gameObject.GetTransform().GetGlobalPosition();

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

	state = States::WANDERING;

}
void EnemyTank::Update()
{
	Recovering();

	switch (state)
	{	
		case States::WANDERING:
			Wander();
		break;
		case States::TARGETING:
			Seek();
		break;
		case States::ATTACKING:
			Attack();
		break;
	}

}
void EnemyTank::Wander() {

	float xDif = gameObject.GetTransform().GetGlobalPosition().x - initialPosition.x;
	//float yDif = gameObject.GetTransform().GetGlobalPosition().y - initialPosition.y;
	float zDif = gameObject.GetTransform().GetGlobalPosition().z - initialPosition.z;
	//Console::Log();
	if ((abs(xDif) > initPosRange) || (abs(zDif) > initPosRange)) {

		API_Vector2 lookDir;
		lookDir.x = (initialPosition.x - gameObject.GetTransform().GetLocalPosition().x);
		lookDir.y = (initialPosition.z - gameObject.GetTransform().GetLocalPosition().z);

		API_Vector2 normLookDir;
		normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
		normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));

		float _angle = 0;
		_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;

		gameObject.GetTransform().SetRotation(0, -_angle, 0);
		enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * walkVelocity);
	}
	else {
		enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * 0);
	}
}
void EnemyTank::Seek() {}
void EnemyTank::Attack() 
{
	if (enemyGun != nullptr)
	{
		enemyGun->Shoot();
	}
}

void EnemyTank::Recovering()
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