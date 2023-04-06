#include "EnemyTank.h"
HELLO_ENGINE_API_C EnemyTank* CreateEnemyTank(ScriptToInspectorInterface* script)
{
	EnemyTank* classInstance = new EnemyTank();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void EnemyTank::Start()
{

}
void EnemyTank::Update()
{

}