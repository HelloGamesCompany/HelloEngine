#include "CollisionTestScript.h"
HELLO_ENGINE_API_C CollisionTestScript* CreateCollisionTestScript(ScriptToInspectorInterface* script)
{
	CollisionTestScript* classInstance = new CollisionTestScript();
	script->AddDragBoxCamera("camera", &classInstance->camera);
	return classInstance;
}

void CollisionTestScript::Start()
{
	this->camera.SetAsActiveCamera();

}
void CollisionTestScript::Update()
{

}

void CollisionTestScript::OnCollisionEnter(API_RigidBody other)
{
	Console::Log("Collision!");

}
