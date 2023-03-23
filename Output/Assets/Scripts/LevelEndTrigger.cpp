#include "LevelEndTrigger.h"
#include "UI Test folder/UIManager.h"
HELLO_ENGINE_API_C LevelEndTrigger* CreateLevelEndTrigger(ScriptToInspectorInterface* script)
{
	LevelEndTrigger* classInstance = new LevelEndTrigger();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("UI manager", &classInstance->UIManagerGO);
	return classInstance;
}

void LevelEndTrigger::Start()
{
	manager = (UIManager*)UIManagerGO.GetScript("UIManager");
}

void LevelEndTrigger::Update()
{

}

void LevelEndTrigger::OnCollisionEnter(API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	if (detectionName == "Player")
		manager->ShowFinalText();
}
