#include "AbilitySelect.h"
HELLO_ENGINE_API_C AbilitySelect* CreateAbilitySelect(ScriptToInspectorInterface* script)
{
	AbilitySelect* classInstance = new AbilitySelect();

	script->AddDragBoxUIInput("Panel Type", &classInstance->abilityTypePanel);
	script->AddDragBoxUIInput("Panel Ability", &classInstance->abilityPanel);
	script->AddDragBoxUIButton("AbilityButton", &classInstance->abilitySelect);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void AbilitySelect::Start()
{
	abilityPanel.SetEnable(false);
}
void AbilitySelect::Update()
{
	if (abilitySelect.OnPress())
	{
		abilityTypePanel.SetEnable(false);
		abilityPanel.SetEnable(true);
	}
	if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN)
	{
		//SelectWeaponList.SetEnable(true);
		//PanelUnlock.SetEnable(false);
	}
}