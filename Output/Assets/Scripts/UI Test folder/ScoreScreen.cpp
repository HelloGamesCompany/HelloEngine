#include "ScoreScreen.h"
HELLO_ENGINE_API_C ScoreScreen* CreateScoreScreen(ScriptToInspectorInterface* script)
{
	ScoreScreen* classInstance = new ScoreScreen();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIButton("Continue", &classInstance->continueButton);
	script->AddDragBoxUIButton("Exit", &classInstance->exitButton);
	return classInstance;
}

void ScoreScreen::Start()
{

}
void ScoreScreen::Update()
{
	if (continueButton.OnPress())
	{
		Scene::LoadScene("SpaceshipHUB_Scene.HScene");
	}
	if (exitButton.OnPress())
	{
		Scene::LoadScene("MainMenu.HScene");
	}
}