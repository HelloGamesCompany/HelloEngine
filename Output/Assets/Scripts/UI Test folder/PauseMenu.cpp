#include "PauseMenu.h"
#include "UIManager.h"
HELLO_ENGINE_API_C PauseMenu* CreatePauseMenu(ScriptToInspectorInterface* script)
{
	PauseMenu* classInstance = new PauseMenu();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("UI Manager: ", &classInstance->uiManager);
	script->AddDragBoxUIButton("Continue: ", &classInstance->continueButton);
	script->AddDragBoxUIButton("Settings: ", &classInstance->settingsButton);
	script->AddDragBoxUIButton("Main Menu: ", &classInstance->mainMenuButton);

	return classInstance;
}

void PauseMenu::Start()
{

}
void PauseMenu::Update()
{
	if (continueButton.OnPress())
	{
		UIManager* manager = (UIManager*)uiManager.GetScript("UIManager");
		manager->ContinueGame();
	}
	else if (settingsButton.OnPress())
	{
		// No settings yet!
	}
	else if (mainMenuButton.OnPress())
	{
		Time::ChangeTimeScale(1.0f);
		Scene::LoadScene("MainMenu.HScene");
	}
}