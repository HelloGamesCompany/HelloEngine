#include "EnemyRanger.h"
#include "Enemy.h"
#include <time.h>
HELLO_ENGINE_API_C EnemyRanger* CreateEnemyRanger(ScriptToInspectorInterface* script)
{
	EnemyRanger* classInstance = new EnemyRanger();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("Detection distance", &classInstance->detectionDis);
	script->AddDragFloat("Lossing Enemy distance", &classInstance->lossingDis);
	script->AddDragFloat("Range attack", &classInstance->disPlayer);
	script->AddDragBoxGameObject("Target", &classInstance->target);
	script->AddDragBoxGameObject("Action zone", &classInstance->actionZone);
	script->AddDragBoxRigidBody("Action Rb zone", &classInstance->zoneRb);
	script->AddDragBoxGameObject("Point 1", &classInstance->listPoints[0]);
	script->AddDragBoxGameObject("Point 2", &classInstance->listPoints[1]);
	script->AddDragBoxGameObject("Point 3", &classInstance->listPoints[2]);
	//script->AddDragBoxGameObject("Point 4", &classInstance->listPoints[3]);
	//script->AddDragBoxGameObject("Point 5", &classInstance->listPoints[4]);
	
	return classInstance;
}

void EnemyRanger::Start()
{
	actualPoint = listPoints[0].GetTransform().GetGlobalPosition();

	_avalPoints = 3;
	enemState = States::WANDERING;
	//clock.s
}
void EnemyRanger::Update()
{
	Enemy* enemy = (Enemy*)gameObject.GetScript("Enemy");
	float dt = Time::GetDeltaTime();

	if (enemy != nullptr)
	{
		float dis = gameObject.GetTransform().GetGlobalPosition().Distance(target.GetTransform().GetGlobalPosition());
		float disZone = gameObject.GetTransform().GetGlobalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());

		if ((dis < detectionDis) && enemState != States::TARGETING)
		{
			enemState = States::TARGETING;
		}
		else if ((dis < disPlayer)  )
		{
			enemState = States::ATTACKIG;
		}
		else if ((dis > lossingDis) || (disZone > zoneRb.GetRadius() / 2))
		{
			enemState = States::WANDERING;
		}

		switch (enemState)
		{
		case States::WANDERING:

			if ((gameObject.GetTransform().GetGlobalPosition().Distance(actualPoint) < 40) /*&& !targeting*/)
			{
				enemy->currentSpeed = enemy->speed * dt;
				numPoint++;
				if (numPoint >= _avalPoints)numPoint = 0;
			}

			actualPoint = listPoints[numPoint].GetTransform().GetGlobalPosition();

			Wander(enemy->currentSpeed, actualPoint);

			break;

		case States::TARGETING:

			enemy->currentSpeed = enemy->speed * enemy->acceleration * dt;
			Seek(enemy->currentSpeed, target.GetTransform().GetGlobalPosition());

			break;

		case States::ATTACKIG:

			enemy->currentSpeed = enemy->speed * enemy->acceleration * dt;
			Attacking(enemy->currentSpeed, target.GetTransform().GetGlobalPosition());

			break;
		default:
			break;
		}
	}
}

void EnemyRanger::Seek(float vel, API_Vector3 tarPos)
{
	API_Vector2 lookDir;
	lookDir.x = (tarPos.x - gameObject.GetTransform().GetGlobalPosition().x);
	lookDir.y = (tarPos.z - gameObject.GetTransform().GetGlobalPosition().z);

	API_Vector2 normLookDir;
	normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	float _angle = 0;
	_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
	gameObject.GetTransform().SetRotation(0, -_angle, 0);

	gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * vel);

}

void EnemyRanger::Wander(float vel, API_Vector3 point)
{
	
	API_Vector2 lookDir;
	lookDir.x = (point.x - gameObject.GetTransform().GetGlobalPosition().x);
	lookDir.y = (point.z - gameObject.GetTransform().GetGlobalPosition().z);

	API_Vector2 normLookDir;
	normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	float _angle = 0;
	_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;

	gameObject.GetTransform().SetRotation(0, -_angle, 0);
	gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * vel);

}

void EnemyRanger::Attacking(float vel, API_Vector3 tarPos)
{
}



API_Vector3 EnemyRanger::NormalizeVec3(float x, float y, float z)
{
	float lenght = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

	x = x / lenght;
	y = y / lenght;
	z = z / lenght;
	return (x, y, z);
}

float EnemyRanger::Lerp(float a, float b, float time)
{
	return a + time * (b - a);
}
