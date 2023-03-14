#include "EnemyMeleeMovement.h"
#include "Enemy.h"
HELLO_ENGINE_API_C EnemyMeleeMovement* CreateEnemyMeleeMovement(ScriptToInspectorInterface* script)
{
	EnemyMeleeMovement* classInstance = new EnemyMeleeMovement();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("Detection distance", &classInstance->detectionDis);
	//script->AddDragBoxGameObject("Aux Cam", &classInstance->finalCam);
	return classInstance;
}

void EnemyMeleeMovement::Start()
{


}
void EnemyMeleeMovement::Update()
{
	Enemy* enemy = (Enemy*)gameObject.AddScript("Enemy");
	
	if (enemy!=nullptr)
	{
		if ((gameObject.GetTransform().GetGlobalPosition().Distance(enemy->target.GetTransform().GetGlobalPosition()) < detectionDis) && !targeting)
		{
			Seek();
		}
		
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

void EnemyMeleeMovement::Seek()
{

}
