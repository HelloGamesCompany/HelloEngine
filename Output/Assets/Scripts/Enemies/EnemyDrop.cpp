#include "EnemyDrop.h"
HELLO_ENGINE_API_C EnemyDrop* CreateEnemyDrop(ScriptToInspectorInterface* script)
{
	EnemyDrop* classInstance = new EnemyDrop();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragInt("Drop Index", &classInstance->dropIndex);
	return classInstance;
}

void EnemyDrop::Start()
{

}

void EnemyDrop::Update()
{

}