#include "PlayerGunManager.h"
HELLO_ENGINE_API_C PlayerGunManager* CreatePlayerGunManager(ScriptToInspectorInterface* script)
{
    PlayerGunManager* classInstance = new PlayerGunManager();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Gun", &classInstance->guns);
    return classInstance;
}

void PlayerGunManager::Start()
{
    PlayerGunType* gunType = (PlayerGunType*)guns.GetScript("PlayerGunType");
    switch (gunType->gunType)
    {
    case 0: // semiautomatic
        equipedGun = (PlayerGun*)guns.GetScript("PlayerSemiAuto");
        break;
    default:
        equipedGun = nullptr;
        break;
    }
}

void PlayerGunManager::Update()
{
    if (equipedGun == nullptr) return;

    if (Input::GetGamePadButton(GamePadButton::BUTTON_RIGHT_SHOULDER) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_SPACE) == KeyState::KEY_DOWN)
    {
        equipedGun->Shoot();
    }
}