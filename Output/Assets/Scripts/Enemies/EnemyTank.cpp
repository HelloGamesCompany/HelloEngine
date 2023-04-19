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
	//script->AddDragBoxGameObject("Enemy To Protect", &classInstance->enemyToProtect);
	script->AddCheckBox("Is Protecting", &classInstance->isProtectingAlly);

	script->AddDragBoxAnimationResource("Idle Animation", &classInstance->idleAnim);
	script->AddDragBoxAnimationResource("Walk Animation", &classInstance->walkAnim);
	script->AddDragBoxAnimationResource("Die Animation", &classInstance->dieAnim);

	script->AddDragBoxAnimationPlayer("Animation", &classInstance->animationPlayer);
	//script->AddDragFloat("Testing1", &classInstance->testingFloat1);
	/*script->AddDragFloat("Testing2", &classInstance->testingFloat2);
	script->AddDragFloat("Testing3", &classInstance->testingFloat3);
	script->AddDragFloat("Testing4", &classInstance->testingFloat4);
	script->AddDragFloat("Testing5", &classInstance->testingFloat5);*/
	return classInstance;
}

void EnemyTank::Start()
{
	
	isReturning = false;
	currentShield = maxShield;
	isRecoveringShield = false;
	isRestoringHealth = false;
	shieldRecoverCounter = 0;
	//alliesDistance = 
	isProtectingAlly = false;
	//animationPlayer = gameObject.GetAnimationPlayer();
	animationPlayer.ChangeAnimation(idleAnim);
	animationPlayer.Play();
	animationPlayer.GetGameObject().GetTransform().SetScale(1.5, 1.5, 1.5);
	animState = AnimStates::IDLE;
	//animationPlayer.GetGameObject().GetTransform().SetPosition(0,-1.8,0);

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
			//Attack();
			break;
		}
	}
	else {
		ReturnToZone();
	}
	
	//A is protrected enemy
	//B is player
	//C is tank

	//API_Vector2 A = (enemyToProtect.GetTransform().GetGlobalPosition().x, enemyToProtect.GetTransform().GetGlobalPosition().z);
	//float afx = enemyToProtect.GetTransform().GetGlobalPosition().x;
	//float afy = enemyToProtect.GetTransform().GetGlobalPosition().z;
	//
	////API_Vector2 B = (target.GetTransform().GetGlobalPosition().x, target.GetTransform().GetGlobalPosition().z);
	////API_Vector2 C = (gameObject.GetTransform().GetGlobalPosition().x, gameObject.GetTransform().GetGlobalPosition().z);

	//////1
	////API_Vector2 AB = (B.x - A.x, B.y - A.y);
	//////2
	////API_Vector2 AC = (C.x - A.x, C.y - A.y);
	//////3
	////float CrossProductABAC = ((AB.x * AC.y) - (AB.y * AC.x));
	//testingFloat1 = A.x;
	//testingFloat2 = A.y;
	//testingFloat3 = afx;
	//testingFloat4 = afy;
	//testingFloat5 = CrossProductABAC;

	

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
		if (animState != AnimStates::WALK)
		{
			animationPlayer.ChangeAnimation(walkAnim);
			animationPlayer.Play();
			animState = AnimStates::WALK;
		}
	}
	else {
		
		enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * 0);
		if (animState != AnimStates::IDLE)
		{
			animationPlayer.ChangeAnimation(idleAnim);
			animationPlayer.Play();
			animState = AnimStates::IDLE;
		}
	}
}

void EnemyTank::Seek() 
{
	if (!enemyScript->actStun)
	{
		if (targetDistance > approximateRange) {
			
			enemyScript->currentSpeed = enemyScript->speed * enemyScript->acceleration * enemyScript->stunVel * enemyScript->slowVel;

			/*if (isProtectingAlly == true) 
			{
				MoveToDirection(target.GetTransform().GetGlobalPosition().x, target.GetTransform().GetGlobalPosition().z, enemyScript->currentSpeed);
			}
			else 
			{
				MoveToDirection(target.GetTransform().GetGlobalPosition().x, target.GetTransform().GetGlobalPosition().z, enemyScript->currentSpeed);
			}*/
			float dirToPlayer[2];
			dirToPlayer[0] = target.GetTransform().GetGlobalPosition().x;
			dirToPlayer[1] = target.GetTransform().GetGlobalPosition().z;

			float dirToPlayerNorm[2];
			dirToPlayerNorm[0] = dirToPlayer[0] / sqrt(pow(dirToPlayer[0], 2) + pow(dirToPlayer[1], 2));
			dirToPlayerNorm[1] = dirToPlayer[1] / sqrt(pow(dirToPlayer[0], 2) + pow(dirToPlayer[1], 2));

			if (isProtectingAlly == true)
			{
				//A is protrected enemy
				//B is player
				//C is tank

				float A[2];
				A[0] = protectedEnemy.GetTransform().GetGlobalPosition().x;
				A[1] = protectedEnemy.GetTransform().GetGlobalPosition().z;

				float B[2];
				B[0] = target.GetTransform().GetGlobalPosition().x;
				B[1] = target.GetTransform().GetGlobalPosition().z;

				float C[2];
				C[0] = gameObject.GetTransform().GetGlobalPosition().x;
				C[1] = gameObject.GetTransform().GetGlobalPosition().z;

				//1 - Ally to player
				float AB[2];
				AB[0] = B[0] - A[0];
				AB[1] = B[1] - A[1];

				//2 - Ally to tank
				float AC[2];
				AC[0] = C[0] - A[0];
				AC[1] = C[1] - A[1];
				
				//Tank to player
				float CB[2] = {0,0};
				CB[0] = B[0] - C[0];
				CB[1] = B[1] - C[1];

				//3
				float CrossProductABAC = ((AB[0] * AC[1]) - (AB[1] * AC[0]));
				testingFloat1 = CrossProductABAC;

				float dirToAlly[2] = { 0,0 };
				bool isInZone = false;
				//tank is on the right - need anti schedule rotated vector
				if (CrossProductABAC > 10) {
					dirToAlly[0] = AB[1];
					dirToAlly[1] = -AB[0];
				}
				//tank is on the right - need schedule rotated vector
				else if (CrossProductABAC < -10) {
					dirToAlly[0] = -AB[1];
					dirToAlly[1] = AB[0];
				}
				else 
				{
					isInZone = true;
				}

				/*float dirToAllyFromTank[2];
				dirToAllyFromTank[0] = dirToAlly[0] + C[0]);
				dirToAllyFromTank[1] = dirToAlly[1] / sqrt(pow(dirToAlly[0], 2) + pow(dirToAlly[1], 2));*/

				float dirToAllyAndPlayer[2] = {0,0};
				if (isInZone == false) 
				{
					float dirToAllyNorm[2];
					dirToAllyNorm[0] = dirToAlly[0] / sqrt(pow(dirToAlly[0], 2) + pow(dirToAlly[1], 2));
					dirToAllyNorm[1] = dirToAlly[1] / sqrt(pow(dirToAlly[0], 2) + pow(dirToAlly[1], 2));

					/*float dirToAllyNorm[2];
					dirToAllyNorm[0] = dirToAlly[0] / sqrt(pow(dirToAlly[0], 2) + pow(dirToAlly[1], 2));
					dirToAllyNorm[1] = dirToAlly[1] / sqrt(pow(dirToAlly[0], 2) + pow(dirToAlly[1], 2));*/

					float CBNorm[2];
					CBNorm[0] = CB[0] / sqrt(pow(CB[0], 2) + pow(CB[1], 2));
					CBNorm[1] = CB[1] / sqrt(pow(CB[0], 2) + pow(CB[1], 2));

					dirToAllyAndPlayer[0] = (dirToAllyNorm[0] + CBNorm[0]) / 2;
					dirToAllyAndPlayer[1] = (dirToAllyNorm[1] + CBNorm[1]) / 2;
				}
				else 
				{
					/*float dirToAllyNorm[2];
					dirToAllyNorm[0] = dirToAlly[0] / sqrt(pow(dirToAlly[0], 2) + pow(dirToAlly[1], 2));
					dirToAllyNorm[1] = dirToAlly[1] / sqrt(pow(dirToAlly[0], 2) + pow(dirToAlly[1], 2));*/

					/*float dirToAllyNorm[2];
					dirToAllyNorm[0] = dirToAlly[0] / sqrt(pow(dirToAlly[0], 2) + pow(dirToAlly[1], 2));
					dirToAllyNorm[1] = dirToAlly[1] / sqrt(pow(dirToAlly[0], 2) + pow(dirToAlly[1], 2));*/

					float CBNorm[2];
					CBNorm[0] = CB[0] / sqrt(pow(CB[0], 2) + pow(CB[1], 2));
					CBNorm[1] = CB[1] / sqrt(pow(CB[0], 2) + pow(CB[1], 2));

					
					dirToAllyAndPlayer[0] = (dirToAlly[0] + CBNorm[0]) / 2;
					dirToAllyAndPlayer[1] = (dirToAlly[1] + CBNorm[1]) / 2;
				}
				

			

				/*float dirToAllyAndPlayer[2];
				dirToAllyAndPlayer[0] = (dirToPlayerNorm[0]) / 2;
				dirToAllyAndPlayer[1] = (dirToPlayerNorm[1]) / 2;*/

			/*	float dirToAllyAndPlayer[2];
				dirToAllyAndPlayer[0] = (dirToAllyNorm[0]) / 2;
				dirToAllyAndPlayer[1] = (dirToAllyNorm[1]) / 2;*/

				float dirToAllyAndPlayerNorm[2];
				dirToAllyAndPlayerNorm[0] = dirToAllyAndPlayer[0] / sqrt(pow(dirToAllyAndPlayer[0], 2) + pow(dirToAllyAndPlayer[1], 2));
				dirToAllyAndPlayerNorm[1] = dirToAllyAndPlayer[1] / sqrt(pow(dirToAllyAndPlayer[0], 2) + pow(dirToAllyAndPlayer[1], 2));


				/*float dirToAllyPlayerFromTank[2];
				dirToAllyPlayerFromTank[0] = dirToAllyAndPlayer[0] + C[0];
				dirToAllyPlayerFromTank[1] = dirToAllyAndPlayer[1] + C[1];*/

				/*float dirToAllyFromTank[2];
				dirToAllyFromTank[0] = dirToAlly[0] + C[0];
				dirToAllyFromTank[1] = dirToAlly[1] + C[1];

				float dirToPlayerFromTank[2];
				dirToPlayerFromTank[0] = dirToPlayer[0] + C[0];
				dirToPlayerFromTank[1] = dirToPlayer[1] + C[1];*/

				//float finalVec[2] = { (dirToAllyFromTank[0] + dirToPlayerFromTank[0]) / 2, (dirToAllyFromTank[1] + dirToPlayerFromTank[1]) / 2 };

				/////*float dirToAllyFromTankNorm[2];
				////dirToAllyFromTankNorm[0] = dirToAllyFromTank[0] / sqrt(pow(dirToAllyFromTank[0], 2) + pow(dirToAllyFromTank[1], 2));
				////dirToAllyFromTankNorm[1] = dirToAllyFromTank[1] / sqrt(pow(dirToAllyFromTank[0], 2) + pow(dirToAllyFromTank[1], 2));

				///float finalVec[2] = { (dirToPlayerNorm[0] + dirToAllyFromTankNorm[0]) / 2, (dirToPlayerNorm[1] + dirToAllyFromTankNorm[1]) / 2 };

				////float finalVecNorm[2];
				////finalVecNorm[0] = finalVec[0] / sqrt(pow(finalVec[0], 2) + pow(finalVec[1], 2));
				////finalVecNorm[1] = finalVec[1] / sqrt(pow(finalVec[0], 2) + pow(finalVec[1], 2));*/


				//MoveToDirection(finalVec[0], finalVec[1], enemyScript->currentSpeed);
				/*float _angle = 0;
				_angle = atan2(finalVec[0], finalVec[1]) * RADTODEG - 90.0f;

				gameObject.GetTransform().SetRotation(0, -_angle, 0);*/
				/*API_Vector3 aaa = (finalVecNorm[0], 0, finalVecNorm[1]);
				enemyScript->enemyRb.SetVelocity(aaa * enemyScript->currentSpeed);*/

				//MoveToDirection(dirToAllyFromTank[0], dirToAllyFromTank[1], enemyScript->currentSpeed);

				//API_Vector2 lookDir;
				///*lookDir.x = (dirToAllyFromTank[0] - gameObject.GetTransform().GetLocalPosition().x);
				//lookDir.y = (dirToAllyFromTank[1] - gameObject.GetTransform().GetLocalPosition().z);*/
				//lookDir.x = (dirToAllyPlayerFromTank[0] - gameObject.GetTransform().GetLocalPosition().x);
				//lookDir.y = (dirToAllyPlayerFromTank[1] - gameObject.GetTransform().GetLocalPosition().z);


				/*API_Vector2 normLookDir;
				normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
				normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));*/

				
				//if (isInZone == false) 
				//{
					//API_Vector3 rot = gameObject.GetTransform().GetGlobalRotation();
					float _angle = 0;
					_angle = atan2(dirToAllyAndPlayerNorm[1], dirToAllyAndPlayerNorm[0]) * RADTODEG - 90.0f;
					//API_Vector3 aaa = (1,0,0);
					gameObject.GetTransform().SetRotation(0, -_angle, 0);
					enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * enemyScript->currentSpeed);

					API_Vector2 setRotToPlayer;
					setRotToPlayer.x = (target.GetTransform().GetGlobalPosition().x - gameObject.GetTransform().GetGlobalPosition().x);
					setRotToPlayer.y = (target.GetTransform().GetGlobalPosition().z - gameObject.GetTransform().GetGlobalPosition().z);

					API_Vector2 normSetRotToPlayer;
					normSetRotToPlayer.x = setRotToPlayer.x / sqrt(pow(setRotToPlayer.x, 2) + pow(setRotToPlayer.y, 2));
					normSetRotToPlayer.y = setRotToPlayer.y / sqrt(pow(setRotToPlayer.x, 2) + pow(setRotToPlayer.y, 2));

					_angle = 0;
					_angle = atan2(normSetRotToPlayer.y, normSetRotToPlayer.x) * RADTODEG - 90.0f;
					//API_Vector3 aaa = (1,0,0);
					gameObject.GetTransform().SetRotation(0, -_angle, 0);

					//float _angle2 = 0;
					//_angle2 = atan2(dirToPlayerNorm[1], dirToPlayerNorm[0]) * RADTODEG - 90.0f;
					////API_Vector3 aaa = (1,0,0);
					//gameObject.GetTransform().SetRotation(0, -_angle2, 0);

					//gameObject.GetTransform().SetRotation(rot);
					//gameObject.GetTransform().SetRotation(0, +2*_angle, 0);
				//}
				//else 
				//{
				//	/*float _angle = 0;
				//	_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;

				//	gameObject.GetTransform().SetRotation(0, -_angle, 0);*/
				//	enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * 0);
				//}
				//gameObject.GetTransform().SetRotation(0, +_angle, 0);
			}
			else
			{
				//B is player
				//C is tank

				float B[2];
				B[0] = target.GetTransform().GetGlobalPosition().x;
				B[1] = target.GetTransform().GetGlobalPosition().z;

				float C[2];
				C[0] = gameObject.GetTransform().GetGlobalPosition().x;
				C[1] = gameObject.GetTransform().GetGlobalPosition().z;

				//Tank to player
				float CB[2] = { 0,0 };
				CB[0] = B[0] - C[0];
				CB[1] = B[1] - C[1];

				MoveToDirection(B[0], B[1], enemyScript->currentSpeed);

				if (animState != AnimStates::WALK)
				{
					animationPlayer.ChangeAnimation(walkAnim);
					animationPlayer.Play();
					animState = AnimStates::WALK;
				}
			}

			//
			//ProtectEnemy();
			
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

void EnemyTank::ProtectEnemy() 
{

	if (isProtectingAlly == false) 
	{
		return; 
	}

	//A is protrected enemy
	//B is player
	//C is tank

	float A[2];
	A[0] = protectedEnemy.GetTransform().GetGlobalPosition().x;
	A[1] = protectedEnemy.GetTransform().GetGlobalPosition().z;

	float B[2];
	B[0] = target.GetTransform().GetGlobalPosition().x;
	B[1] = target.GetTransform().GetGlobalPosition().z;

	float C[2];
	C[0] = gameObject.GetTransform().GetGlobalPosition().x;
	C[1] = gameObject.GetTransform().GetGlobalPosition().z;

	//1
	float AB[2];
	AB[0] = B[0] - A[0];
	AB[1] = B[1] - A[1];

	//2
	float AC[2];
	AC[0] = C[0] - A[0];
	AC[1] = C[1] - A[1];

	//3
	float CrossProductABAC = ((AB[0] * AC[1]) - (AB[1] * AC[0]));
	testingFloat1 = CrossProductABAC;

	float rotatedVec[2] = { 0,0 };
	//tank is on the right - need anti schedule rotated vector
	if (CrossProductABAC > 0) {
		rotatedVec[0] = AB[1];
		rotatedVec[1] = -AB[0];


		MoveToDirection(rotatedVec[0], rotatedVec[1], enemyScript->currentSpeed);

		////float dirX = rotatedVec[0] - gameObject.GetTransform().GetGlobalPosition().x;
		////float dirY = rotatedVec[1] - gameObject.GetTransform().GetGlobalPosition().x;
		////float normDirX = dirX / sqrt(pow(dirX, 2) + pow(dirY, 2));
		////float normDirY = dirY / sqrt(pow(dirX, 2) + pow(dirY, 2));

		/////*API_Vector2 lookDir;
		////lookDir.x = (rotatedVec[0] - gameObject.GetTransform().GetLocalPosition().x);
		////lookDir.y = (rotatedVec[1] - gameObject.GetTransform().GetLocalPosition().z);

		////API_Vector2 normLookDir;
		////normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
		////normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));

		////float _angle = 0;
		////_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;*/

		//////gameObject.GetTransform().SetRotation(0, -_angle, 0);

		////API_Vector3 aaa = (normDirX, 0, normDirY);
		//////enemyScript->enemyRb.SetVelocity(aaa * enemyScript->currentSpeed/2);
		////enemyScript->enemyRb.SetVelocity(aaa * enemyScript->currentSpeed);
		//gameObject.GetTransform().SetRotation(0, +_angle, 0);
	}
	//tank is on the right - need schedule rotated vector
	else if (CrossProductABAC < 0) {
		rotatedVec[0] = -AB[1];
		rotatedVec[1] = AB[0];

		MoveToDirection(rotatedVec[0], rotatedVec[1], enemyScript->currentSpeed);
		//float dirX = rotatedVec[0] - gameObject.GetTransform().GetGlobalPosition().x;
		//float dirY = rotatedVec[1] - gameObject.GetTransform().GetGlobalPosition().x;
		//float normDirX = dirX / sqrt(pow(dirX, 2) + pow(dirY, 2));
		//float normDirY = dirY / sqrt(pow(dirX, 2) + pow(dirY, 2));

		///*API_Vector2 lookDir;
		//lookDir.x = (rotatedVec[0] - gameObject.GetTransform().GetLocalPosition().x);
		//lookDir.y = (rotatedVec[1] - gameObject.GetTransform().GetLocalPosition().z);

		//API_Vector2 normLookDir;
		//normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
		//normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));

		//float _angle = 0;
		//_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;*/

		////gameObject.GetTransform().SetRotation(0, -_angle, 0);

		//API_Vector3 aaa = (normDirX, 0, normDirY);
		////enemyScript->enemyRb.SetVelocity(aaa * enemyScript->currentSpeed/2);
		//enemyScript->enemyRb.SetVelocity(aaa * enemyScript->currentSpeed);
		////gameObject.GetTransform().SetRotation(0, +_angle, 0);
	}

	if (animState!= AnimStates::WALK) 
	{
		animationPlayer.ChangeAnimation(walkAnim);
		animationPlayer.Play();
		animState = AnimStates::WALK;
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

//void EnemyTank::OnCollisionEnter(API::API_RigidBody other)
//{
//	std::string detectionTag = other.GetGameObject().GetTag();
//	if (detectionTag == "Enemy")
//	{
//		//bool enemyOnList = false;
//		//for (int i = 0; i < allyEnemies.size(); i++) 
//		//{
//		//	//	if (allyEnemies.at(i) == other.GetGameObject()) {}
//
//		//	/*std::string enemyTag = allyEnemies.at(i).GetTag();
//		//	if (enemyTag == "")
//		//	{
//		//	
//		//	}*/
//		//}
//		if (protectedEnemy.GetScript("Enemy") != null)
//		{
//		
//		}
//	}
//}

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

void EnemyTank::DieTank()
{
	enemyScript->_coldAnimDie += Time::GetDeltaTime();
	// enemy->dying = true;
	enemyScript->enemyRb.SetVelocity(0);
	if (enemyScript->_coldAnimDie < enemyScript->_tAnimDie)
	{
		if (animState != AnimStates::DIE)
		{
			animState = AnimStates::DIE;
			animationPlayer.ChangeAnimation(dieAnim);
			animationPlayer.Play();
		}
	}
	else if (enemyScript->_coldAnimDie >= enemyScript->_tAnimDie)
	{
		gameObject.SetActive(false);
	}
}