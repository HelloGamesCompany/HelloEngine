#include "OpenMenuInterruptor.h"
#include "../Player/PlayerMove.h"
HELLO_ENGINE_API_C OpenMenuInterruptor* CreateOpenMenuInterruptor(ScriptToInspectorInterface* script)
{
    OpenMenuInterruptor* classInstance = new OpenMenuInterruptor();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragInt("Number of menus", &classInstance->numberOfAsignedMenus);
    script->AddDragBoxGameObject("Menu to Open 1", &classInstance->menu[0]);
    script->AddDragBoxGameObject("Menu to Open 2", &classInstance->menu[1]);
    script->AddDragBoxGameObject("Menu to Open 3", &classInstance->menu[2]);
    script->AddDragBoxGameObject("Menu to Open 4", &classInstance->menu[3]);
    script->AddDragBoxGameObject("Menu to Open 5", &classInstance->menu[4]);
    script->AddDragBoxGameObject("Menu to Open 6", &classInstance->menu[5]);
    script->AddDragBoxGameObject("Menu to Open 7", &classInstance->menu[6]);
    script->AddDragBoxGameObject("Menu to Open 8", &classInstance->menu[7]);
    script->AddDragBoxGameObject("Menu to Open 9", &classInstance->menu[8]);
    script->AddDragBoxGameObject("Menu to Open 10", &classInstance->menu[9]);
    script->AddDragBoxGameObject("Armory Panel", &classInstance->armoryPanel);
    return classInstance;
}

void OpenMenuInterruptor::Start()
{

}

void OpenMenuInterruptor::Update()
{

}

void OpenMenuInterruptor::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        if (open) return;

        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_DOWN)
        {
            playerMove = (PlayerMove*)other.GetGameObject().GetScript("PlayerMove");
            if (playerMove == nullptr) return;

            float distanceX = gameObject.GetTransform().GetGlobalPosition().x - other.GetGameObject().GetTransform().GetGlobalPosition().x;
            float distanceZ = gameObject.GetTransform().GetGlobalPosition().z - other.GetGameObject().GetTransform().GetGlobalPosition().z;

            OpenMenus();
            open = true;
        }
    }
}

void OpenMenuInterruptor::OpenMenus()
{
    for (size_t i = 0; i < numberOfAsignedMenus; i++)
    {
        menu[i].SetActive(true);
    }
}