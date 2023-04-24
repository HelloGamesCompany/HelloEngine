#include "AbilityDescription.h"
HELLO_ENGINE_API_C AbilityDescription* CreateAbilityDescription(ScriptToInspectorInterface* script)
{
	AbilityDescription* classInstance = new AbilityDescription();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Current Description", &classInstance->currentDescription);
	script->AddDragBoxGameObject("Next Description", &classInstance->nextDescription);
	script->AddDragBoxGameObject("Previous Description", &classInstance->previousDescription);
	script->AddDragBoxUIButton("Button", &classInstance->HoveredButton);
	return classInstance;
}

void AbilityDescription::Start()
{
	HoveredButton.SetBlocked(true);
}
void AbilityDescription::Update()
{
	if (HoveredButton.getIsBlocked() || HoveredButton.OnHovered()) {
		previousDescription.SetActive(false);
		currentDescription.SetActive(true);
		nextDescription.SetActive(false);
		Console::Log("1");
	}
}