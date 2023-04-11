#include "AbilityTreeUpgrades.h"
HELLO_ENGINE_API_C AbilityTreeUpgrades* CreateAbilityTreeUpgrades(ScriptToInspectorInterface* script)
{
	AbilityTreeUpgrades* classInstance = new AbilityTreeUpgrades();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIButton("Ability1", &classInstance->buttonHovered);

	script->AddDragBoxGameObject("DescriptionHovered", &classInstance->description);
	script->AddDragBoxGameObject("DescriptionBack", &classInstance->descriptionBack);
	script->AddDragBoxGameObject("DescriptionFoward", &classInstance->descriptionFoward);
	return classInstance;
}

void AbilityTreeUpgrades::Start()
{
	description.SetActive(false);
	descriptionBack.SetActive(false);
	descriptionFoward.SetActive(false);
}
void AbilityTreeUpgrades::Update()
{
	if (buttonHovered.OnHovered())
	{
		description.SetActive(true);
		descriptionBack.SetActive(false);
		descriptionFoward.SetActive(false);
	}
	if (buttonHovered.OnPress())
	{
		buttonHovered.SetBlocked(true);
	}
}