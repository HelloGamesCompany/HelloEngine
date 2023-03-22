#include "EnemyMeleeMovement.h"
#include "Enemy.h"
#include <time.h>

HELLO_ENGINE_API_C EnemyMeleeMovement* CreateEnemyMeleeMovement(ScriptToInspectorInterface* script)
{
	EnemyMeleeMovement* classInstance = new EnemyMeleeMovement();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("Detection distance", &classInstance->detectionDis);
	script->AddDragFloat("Lossing Enemy distance", &classInstance->lossingDis);
	//script->AddDragFloat("Lossing Zone distance", &classInstance->lossingZoneDis);
	script->AddDragBoxGameObject("Target", &classInstance->target);
	//script->AddDragBoxGameObject("Point 1", &classInstance->point1);
	//script->AddDragBoxGameObject("Point 2", &classInstance->point2);
	script->AddDragBoxGameObject("Action zone", &classInstance->actionZone);
	script->AddDragBoxRigidBody("Action Rb zone", &classInstance->zoneRb);
	script->AddDragBoxGameObject("Point 1", &classInstance->listPoints[0]);
	script->AddDragBoxGameObject("Point 2", &classInstance->listPoints[1]);
	script->AddDragBoxGameObject("Point 3", &classInstance->listPoints[2]);
	//script->AddDragBoxGameObject("Point 4", &classInstance->listPoints[3]);
	//script->AddDragBoxGameObject("Point 5", &classInstance->listPoints[4]);
	

	return classInstance;
}

void EnemyMeleeMovement::Start()
{
	
	actualPoint = listPoints[0].GetTransform().GetGlobalPosition() ;
	
	_avalPoints = 3;
	enemState = States::WANDERING;
	//clock.s
	
} 
void EnemyMeleeMovement::Update()
{
	Enemy* enemy = (Enemy*)gameObject.GetScript("Enemy");
	float dt = Time::GetDeltaTime();

	if (enemy!=nullptr)
	{	
		float dis = gameObject.GetTransform().GetGlobalPosition().Distance(target.GetTransform().GetGlobalPosition());
		float disZone = gameObject.GetTransform().GetGlobalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());

		if ((dis< detectionDis) && enemState != States::TARGETING)
		{
			enemState = States::TARGETING;
			//targeting = true;
		}
		else if ((dis>lossingDis) || (disZone > zoneRb.GetRadius()/2) )
		{
			enemState = States::WANDERING;
			//targeting = false;
		}
		
		
		switch (enemState)
		{
		case States::WANDERING:

				enemy->currentSpeed = enemy->speed * dt;
			if ((gameObject.GetTransform().GetGlobalPosition().Distance(actualPoint) < 40) /*&& !targeting*/)
			{
				/*if (numPoint == 1)numPoint = 2, actualPoint=point2.GetTransform().GetGlobalPosition();
				else if (numPoint == 2)numPoint = 1, actualPoint = point1.GetTransform().GetGlobalPosition();*/
				//++numPoint;

				//if (numPoint != _avalPoints) numPoint++/*, actualPoint = listPoints[numPoint].GetTransform().GetGlobalPosition()*/;
				//else if (numPoint == _avalPoints)numPoint = 0/*, actualPoint = listPoints[numPoint].GetTransform().GetGlobalPosition()*/;

				numPoint++;
				if (numPoint >= _avalPoints)numPoint = 0;
			}

			//if (!targeting)
			//{
				/*if (numPoint == 1) actualPoint = point2.GetTransform().GetGlobalPosition();
				else if (numPoint == 2) actualPoint = point1.GetTransform().GetGlobalPosition();*/
			actualPoint = listPoints[numPoint].GetTransform().GetGlobalPosition();
				//if (numPoint != _avalPoints) actualPoint = listPoints[numPoint].GetTransform().GetGlobalPosition();
				//else if (numPoint == _avalPoints) actualPoint = listPoints[numPoint].GetTransform().GetGlobalPosition();
			//}

				Wander(enemy->currentSpeed, actualPoint);

			break;

		case States::TARGETING:
			enemy->currentSpeed = enemy->speed * enemy->acceleration * dt;
			Seek(enemy->currentSpeed, target.GetTransform().GetGlobalPosition());
			break;

		case States::ATTACKIG:

			break;
		default:
			break;
		}
		//targeting ? Seek(enemy->speed, target.GetTransform().GetGlobalPosition()) : Wander(enemy->speed, actualPoint);

	}
}


void EnemyMeleeMovement::Seek(float vel, API_Vector3 tarPos)
{
	API_Vector2 lookDir;
	lookDir.x = (tarPos.x - gameObject.GetTransform().GetGlobalPosition().x);
	lookDir.y = (tarPos.z - gameObject.GetTransform().GetGlobalPosition().z);
	
	API_Vector2 normLookDir;
	normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	float _angle=0;
	_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
	gameObject.GetTransform().SetRotation(0,-_angle,0);

	gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * vel);

	
}

void EnemyMeleeMovement::Wander(float vel, API_Vector3 point)
{
	//if(gameObject.GetTransform().GetGlobalRotation().y>0)gameObject.GetTransform().SetRotation(0, 360, 0),Console::Log("aaaa	");
	//else if(gameObject.GetTransform().GetGlobalRotation().y < 0)gameObject.GetTransform().SetRotation(0, 360, 0);
	API_Vector2 lookDir;
	lookDir.x = (point.x - gameObject.GetTransform().GetGlobalPosition().x);
	lookDir.y = (point.z - gameObject.GetTransform().GetGlobalPosition().z);
	
	API_Vector2 normLookDir;
	normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	float _angle=0;
	_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
	//float wantAngle = Lerp(gameObject.GetTransform().GetGlobalRotation().y,_angle,200000);
	//if(gameObject.GetTransform().GetGlobalRotation().y>=_angle+5 && gameObject.GetTransform().GetGlobalRotation().y <= _angle - 5)gameObject.GetTransform().SetRotation(0,-_angle,0);
	/*if(gameObject.GetTransform().GetGlobalRotation().y == _angle )gameObject.GetTransform().SetRotation(0,-_angle,0);
	else gameObject.GetTransform().Rotate(0,1,0);*/
	//if ((gameObject.GetTransform().GetGlobalRotation().y )/* || (gameObject.GetTransform().GetGlobalRotation().y > (_angle - 4))*/)
	//{
	//	gameObject.GetTransform().Rotate(0, 2, 0);
	//}
	//else {

	//	gameObject.GetTransform().SetRotation(0, -_angle, 0);
	//	gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * vel);
	//}

	gameObject.GetTransform().SetRotation(0, -_angle, 0);
	gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * vel);

}

API_Vector3 EnemyMeleeMovement::NormalizeVec3(float x, float y, float z)
{
	float lenght = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	
	x = x / lenght;
	y = y / lenght;
	z = z / lenght;
	return ( x,y,z );
}

float EnemyMeleeMovement::Lerp(float a, float b, float time)
{
	return a + time * (b - a);
}