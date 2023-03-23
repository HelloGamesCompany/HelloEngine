#include "PauseButtons.h"
HELLO_ENGINE_API_C PauseButtons* CreatePauseButtons(ScriptToInspectorInterface* script)
{
	PauseButtons* classInstance = new PauseButtons();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Panel", &classInstance->Panel);
	script->AddDragBoxUIButton("Continue", &classInstance->Continue);
	script->AddDragBoxUIButton("Main Menu", &classInstance->MainMenu);
	script->AddDragBoxUIButton("Settings", &classInstance->Settings);

	return classInstance;
}

void PauseButtons::Start()
{

}
void PauseButtons::Update()
{
	if (Input::GetGamePadButton(GamePadButton::BUTTON_A) == KeyState::KEY_REPEAT)
	{
		Panel.SetActive(true);
	}
	if (Continue.OnPress())
	{
		Console::Log("im pressed");
		Panel.SetActive(false);
	}
	if (Settings.OnPress())
	{
		Console::Log("im pressed");
		Scene::LoadScene("");
	}
	if (MainMenu.OnPress())
	{
		Console::Log("im pressed");
		Scene::LoadScene("MainMenu.HScene");
	}
	
}