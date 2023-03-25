#include "Level2EndTrigger.h"
HELLO_ENGINE_API_C Level2EndTrigger* CreateLevel2EndTrigger(ScriptToInspectorInterface* script)
{
	Level2EndTrigger* classInstance = new Level2EndTrigger();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Final text panel: ", &classInstance->finalTextPanel);
	return classInstance;
}

void Level2EndTrigger::Start()
{

}
void Level2EndTrigger::Update()
{

}

void Level2EndTrigger::OnCollisionEnter(API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	if (detectionName == "Player")
	{
		finalTextPanel.SetActive(true);
	}
}
