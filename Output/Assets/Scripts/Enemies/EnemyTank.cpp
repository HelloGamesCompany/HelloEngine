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
	/*script->AddDragFloat("Walk Velocity", &classInstance->walkVelocity);
	script->AddDragFloat("Seek Velocity", &classInstance->seekVelocity);*/

	script->AddDragInt("Gun Type(0Semi/1Burst/2Shotgun)", &classInstance->gunType);
	script->AddDragBoxGameObject("Enemy gun", &classInstance->gunObj);

	script->AddDragBoxGameObject("Target", &classInstance->target);
	script->AddDragBoxRigidBody("Action Rb zone", &classInstance->zoneRb);
	script->AddDragFloat("Attack Range", &classInstance->attackRange);
	script->AddDragFloat("Approximate Range", &classInstance->approximateRange);
	script->AddDragFloat("Separate Range", &classInstance->separateRange);

	script->AddDragFloat("Return Distance", &classInstance->returnToZoneDistance);
	//script->AddCheckBox("Test", &classInstance->test);

	return classInstance;
}

void EnemyTank::Start()
{
	isReturning = false;
	currentShield = maxShield;
	isRecoveringShield = false;
	isRestoringHealth = false;
	shieldRecoverCounter = 0;

	enemyScript = (Enemy*)gameObject.GetScript("Enemy");
	if (enemyScript->hasShield == false) {
		enemyScript->hasShield = true;
	}

	initialPosition = gameObject.GetTransform().GetGlobalPosition();
	actionZone = zoneRb.GetGameObject();

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

	CheckDistance();

	if (isReturning == false) {
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
	else {
		ReturnToZone();
	}
	

}
void EnemyTank::ReturnToZone() {

	float xDif = gameObject.GetTransform().GetGlobalPosition().x - actionZone.GetTransform().GetGlobalPosition().x;
	float zDif = gameObject.GetTransform().GetGlobalPosition().z - actionZone.GetTransform().GetGlobalPosition().z;

	if ((abs(xDif) > initPosRange) || (abs(zDif) > initPosRange)) {
		enemyScript->currentSpeed = enemyScript->speed * enemyScript->acceleration * enemyScript->stunVel * enemyScript->slowVel;
		MoveToDirection(actionZone.GetTransform().GetGlobalPosition().x, actionZone.GetTransform().GetGlobalPosition().z, enemyScript->currentSpeed);
	}
	else {
		enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * 0);
	}

	float enemyZoneDistance = gameObject.GetTransform().GetGlobalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());
	if (enemyZoneDistance < (zoneRb.GetRadius() / 2.f) - returnToZoneDistance)
	{
		isReturning = false;
	}
}

void EnemyTank::CheckDistance() {

	float targetZoneDistance = target.GetTransform().GetGlobalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());
	float enemyZoneDistance = gameObject.GetTransform().GetGlobalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());
	if (targetZoneDistance > zoneRb.GetRadius() / 2.f) {
		state = States::WANDERING;
	}
	else if (enemyZoneDistance > zoneRb.GetRadius() / 2.f) {
		state = States::WANDERING;
		isReturning = true;
	}
	else {
		targetDistance = gameObject.GetTransform().GetGlobalPosition().Distance(target.GetTransform().GetGlobalPosition());
		if (targetDistance < detectionDistance) {
			state = States::TARGETING;
		}
		else {
			state = States::WANDERING;
		}
		/*else if ((targetDistance <= attackRange) && (targetDistance >= separateRange)) {
			state = States::ATTACKING;
		}
		state = States::TARGETING;*/
	}
}

void EnemyTank::Wander() {

	float xDif = gameObject.GetTransform().GetGlobalPosition().x - initialPosition.x;
	float zDif = gameObject.GetTransform().GetGlobalPosition().z - initialPosition.z;

	if ((abs(xDif) > initPosRange) || (abs(zDif) > initPosRange)) {

		//enemy->currentSpeed = enemy->speed * enemy->acceleration * enemy->stunVel * enemy->slowVel/** dt*/;
		enemyScript->currentSpeed = enemyScript->speed * enemyScript->stunVel * enemyScript->slowVel;
		MoveToDirection(initialPosition.x, initialPosition.z, enemyScript->currentSpeed);
		//test = true;
	}
	else {
		
		enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * 0);
	}
}

void EnemyTank::Seek() 
{
	if (!enemyScript->actStun)
	{
		if (targetDistance > approximateRange) {
			enemyScript->currentSpeed = enemyScript->speed * enemyScript->acceleration * enemyScript->stunVel * enemyScript->slowVel;
			MoveToDirection(target.GetTransform().GetGlobalPosition().x, target.GetTransform().GetGlobalPosition().z, enemyScript->currentSpeed);
			//Attack();
		}
		else if(targetDistance < separateRange) {
			enemyScript->currentSpeed = enemyScript->speed * enemyScript->acceleration * enemyScript->stunVel * enemyScript->slowVel;
			EscapeFromDirection(target.GetTransform().GetGlobalPosition().x, target.GetTransform().GetGlobalPosition().z, enemyScript->currentSpeed);
		}
		else {
			enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * 0);
		}

		if (targetDistance < attackRange) {
			Attack();
		}
		
	}
}

void EnemyTank::Attack() 
{
	if (enemyGun != nullptr)
	{
		enemyGun->Shoot();
	}
}

void EnemyTank::MoveToDirection(float pointX, float pointY, float velocity)
{
	API_Vector2 lookDir;
	lookDir.x = (pointX - gameObject.GetTransform().GetLocalPosition().x);
	lookDir.y = (pointY - gameObject.GetTransform().GetLocalPosition().z);

	API_Vector2 normLookDir;
	normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));

	float _angle = 0;
	_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;

	gameObject.GetTransform().SetRotation(0, -_angle, 0);
	enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * velocity);
	//test = true;
}

void EnemyTank::EscapeFromDirection(float pointX, float pointY, float velocity)
{
	API_Vector2 lookDir;
	lookDir.x = (pointX - gameObject.GetTransform().GetLocalPosition().x);
	lookDir.y = (pointY - gameObject.GetTransform().GetLocalPosition().z);

	API_Vector2 normLookDir;
	normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));

	float _angle = 0;
	_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;

	gameObject.GetTransform().SetRotation(0, -_angle, 0);
	enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetBackward() * velocity);
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