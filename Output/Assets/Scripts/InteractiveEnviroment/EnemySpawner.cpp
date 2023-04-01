#include "EnemySpawner.h"
HELLO_ENGINE_API_C EnemySpawner* CreateEnemySpawner(ScriptToInspectorInterface* script)
{
	EnemySpawner* classInstance = new EnemySpawner();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void EnemySpawner::Start()
{

}
void EnemySpawner::Update()
{

}