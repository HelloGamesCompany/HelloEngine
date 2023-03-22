#include "GamepadButtons.h"
HELLO_ENGINE_API_C GamepadButtons* CreateGamepadButtons(ScriptToInspectorInterface* script)
{
	GamepadButtons* classInstance = new GamepadButtons();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Pause Panel", &classInstance->Panel_Pause);
	script->AddDragBoxGameObject("Map Panel", &classInstance->Panel_Map);
	return classInstance;
}

void GamepadButtons::Start()
{

}
void GamepadButtons::Update()
{
	if (Input::GetGamePadButton(GamePadButton::BUTTON_START) == KeyState::KEY_DOWN)
	{
		Panel_Pause.SetActive(true);
	}
	if (Input::GetGamePadButton(GamePadButton::BUTTON_LEFT) == KeyState::KEY_DOWN)
	{
		Panel_Map.SetActive(show_map);
		show_map = !show_map;
	}
	
}