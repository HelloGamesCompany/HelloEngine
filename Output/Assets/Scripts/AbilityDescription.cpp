#include "AbilityDescription.h"
HELLO_ENGINE_API_C AbilityDescription* CreateAbilityDescription(ScriptToInspectorInterface* script)
{
	AbilityDescription* classInstance = new AbilityDescription();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Current Description", &classInstance->currentDescription);
	script->AddDragBoxGameObject("Next Description", &classInstance->nextDescription);
	script->AddDragBoxGameObject("Previous Description", &classInstance->previousDescription);
	script->AddDragBoxGameObject("Description 4", &classInstance->description4);
	script->AddDragBoxGameObject("Description 5", &classInstance->description5);
	script->AddDragBoxUIButton("Button", &classInstance->HoveredButton);
	return classInstance;
}

void AbilityDescription::Start()
{

}
void AbilityDescription::Update()
{
	if (HoveredButton.OnHovered()) {
		previousDescription.SetActive(false);
		currentDescription.SetActive(true);
		nextDescription.SetActive(false);
		description4.SetActive(false);
		description5.SetActive(false);
		Console::Log("1");
	}
}