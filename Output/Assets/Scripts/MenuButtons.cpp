#include "MenuButtons.h"

HELLO_ENGINE_API_C MenuButtons* CreateMenuButtons(ScriptToInspectorInterface* script)
{
	MenuButtons* classInstance = new MenuButtons();
	script->AddDragBoxUIButton("Continue", &classInstance->Continue);
	script->AddDragBoxUIButton("NewGame", &classInstance->NewGame);
	script->AddDragBoxUIButton("Exit", &classInstance->Exit);
	script->AddDragBoxUIButton("Settings", &classInstance->Settings);
	script->AddDragBoxUIButton("Collectibles", &classInstance->Collectibles);

	return classInstance;
}

void MenuButtons::Start()
{

}
void MenuButtons::Update()
{
	if (Continue.OnPress())
	{
		Console::Log("im pressed");
		Scene::LoadScene("HUD_Scene.HScene");
	}
	if (NewGame.OnPress())
	{
		Console::Log("im pressed");
		Scene::LoadScene("");
	}
	if (Exit.OnPress())
	{
		Console::Log("im pressed");
		Scene::LoadScene("");
	}
	if (Collectibles.OnPress())
	{
		Console::Log("im pressed");
		Scene::LoadScene("");
	}
	if (Settings.OnPress())
	{
		Console::Log("im pressed");
		Scene::LoadScene("");
	}
}