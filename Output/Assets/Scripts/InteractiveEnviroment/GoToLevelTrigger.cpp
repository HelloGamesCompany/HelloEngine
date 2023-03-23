#include "GoToLevelTrigger.h"
HELLO_ENGINE_API_C GoToLevelTrigger* CreateGoToLevelTrigger(ScriptToInspectorInterface* script)
{
	GoToLevelTrigger* classInstance = new GoToLevelTrigger();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddInputBox("Scene path and name: ", &classInstance->scene);
	return classInstance;
}

void GoToLevelTrigger::Start()
{

}
void GoToLevelTrigger::Update()
{

}

void GoToLevelTrigger::OnCollisionEnter(API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	if (detectionName == "Player")
	{
		Scene::LoadScene(scene.c_str());
	}
}
