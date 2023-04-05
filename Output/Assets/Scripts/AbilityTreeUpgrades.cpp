#include "AbilityTreeUpgrades.h"
HELLO_ENGINE_API_C AbilityTreeUpgrades* CreateAbilityTreeUpgrades(ScriptToInspectorInterface* script)
{
	AbilityTreeUpgrades* classInstance = new AbilityTreeUpgrades();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIButton("Ability1", &classInstance->Ability1);
	script->AddDragBoxUIButton("Ability2", &classInstance->Ability2);
	script->AddDragBoxUIButton("Ability3", &classInstance->Ability3);
	script->AddDragBoxUIButton("Ability4", &classInstance->Ability4);
	return classInstance;
}

void AbilityTreeUpgrades::Start()
{

}
void AbilityTreeUpgrades::Update()
{

}