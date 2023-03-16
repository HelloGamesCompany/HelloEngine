#include "EnemyMeleeMovement.h"
#include "Enemy.h"
HELLO_ENGINE_API_C EnemyMeleeMovement* CreateEnemyMeleeMovement(ScriptToInspectorInterface* script)
{
	EnemyMeleeMovement* classInstance = new EnemyMeleeMovement();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("Detection distance", &classInstance->detectionDis);
	script->AddDragFloat("Lossing distance", &classInstance->lossingDis);
	script->AddDragBoxGameObject("Target", &classInstance->target);
	//script->AddDragBoxGameObject("Aux Cam", &classInstance->finalCam);
	return classInstance;
}

void EnemyMeleeMovement::Start()
{


}
void EnemyMeleeMovement::Update()
{
	Enemy* enemy = (Enemy*)gameObject.GetScript("Enemy");
	float dt = Time::GetDeltaTime();
	if (enemy!=nullptr)
	{
		
		float dis = gameObject.GetTransform().GetGlobalPosition().Distance(target.GetTransform().GetGlobalPosition());
		//Console::Log(std::to_string(dis));
		if ((dis< detectionDis) && !targeting)
		{
			targeting = true;
		}
		else if (dis>lossingDis)
		{
			targeting = false;
		}
		
		enemy->speed =enemy->acceleration* dt;

		targeting ? Seek(enemy->speed, target.GetTransform().GetGlobalPosition()) : Wander(enemy->speed);

		//gameObject.GetTransform().GetGlobalPosition().Distance();
	//	std::distance(gameObject.GetTransform().GetGlobalPosition(), gameObject.GetTransform().GetGlobalPosition());
		//gameObject.GetTransform().GetGlobalPosition().S_Up();
	}
}

float EnemyMeleeMovement::DisanceToObj(API_Vector3 obj1, API_Vector3 obj2)
{
	float dis = 0.0f;
//	dis = sqrt(pow(obj2. - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2))
	return 0.0f;
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

	
	API_Vector3 dir = tarPos - gameObject.GetTransform().GetGlobalPosition();
	dir.y = 0;
	
	Console::Log("-------------");
	Console::Log(std::to_string(dir.x));
	Console::Log(std::to_string(dir.y));
	Console::Log(std::to_string(dir.z));
	
	dir = NormalizeVec3(dir.x,dir.y,dir.z);
	 
	/*Console::Log("xxxxxxxxxx");
	Console::Log(std::to_string(dir.x));  
	Console::Log(std::to_string(dir.y));
	Console::Log(std::to_string(dir.z));*/
	
	
	gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * vel);
	//gameObject.GetTransform().Translate(dir.x * vel, 0, dir.z * vel);
	//gameObject.GetTransform().SetPosition(dir.x * vel, 0, dir.z * vel);

	

}

void EnemyMeleeMovement::Wander(float vel)
{

}

API_Vector3 EnemyMeleeMovement::NormalizeVec3(float x, float y, float z)
{
	float lenght = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	
	x = x / lenght;
	y = y / lenght;
	z = z / lenght;
	return ( x,y,z );
}