#include "CollisionTestScript.h"
HELLO_ENGINE_API_C CollisionTestScript* CreateCollisionTestScript(ScriptToInspectorInterface* script)
{
	CollisionTestScript* classInstance = new CollisionTestScript();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void CollisionTestScript::Start()
{

}
void CollisionTestScript::Update()
{

}

void CollisionTestScript::OnCollisionEnter(API_RigidBody other)
{
	Console::Log("Collision!");

}
