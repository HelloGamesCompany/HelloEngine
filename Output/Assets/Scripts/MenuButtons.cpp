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
        /*Scene::LoadScene("WinMenu.HScene");*/
    }
    if (NewGame.OnPress())
    {
        API_QuickSave::ResetAll();
        API_QuickSave::SetInt("equipedNormalGun", 2); // starter weapon
        Scene::LoadScene("SpaceshipHUB_Scene.HScene");
    }
    if (Exit.OnPress())
    {
        Game::ExitApplication();
    }
    if (Collectibles.OnPress())
    {
        Scene::LoadScene("");
    }
    if (Settings.OnPress())
    {
        Scene::LoadScene("");
    }
}