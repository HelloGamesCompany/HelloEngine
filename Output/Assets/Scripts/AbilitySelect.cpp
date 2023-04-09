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
	Console::Log("Enter");
	if (abilitySelect.OnPress())
	{
		abilityTypePanel.SetEnable(false);
		abilityPanel.SetEnable(true);
		setEnablePanel = true;
		Console::Log("Enter");
	}
	if ((Input::GetKey(KeyCode::KEY_SPACE) == KeyState::KEY_DOWN || (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN)) && setEnablePanel == true)
	{
		setEnablePanel = false;
		abilityTypePanel.SetEnable(true);
		abilityPanel.SetEnable(false);
		Console::Log("Exit");
	}
}