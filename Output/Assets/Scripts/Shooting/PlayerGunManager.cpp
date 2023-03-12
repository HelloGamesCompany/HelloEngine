#include "PlayerGunManager.h"
HELLO_ENGINE_API_C PlayerGunManager* CreatePlayerGunManager(ScriptToInspectorInterface* script)
{
    PlayerGunManager* classInstance = new PlayerGunManager();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Gun1", &classInstance->firstGun);
    script->AddDragBoxGameObject("Gun2", &classInstance->secondGun);
    script->AddDragBoxGameObject("Gun3", &classInstance->thirdGun);
    return classInstance;
}

void PlayerGunManager::Start()
{
    guns.push_back(firstGun);
    guns.push_back(secondGun);
    guns.push_back(thirdGun);

    EquipGun(0);
}

void PlayerGunManager::Update()
{
    if (Input::GetGamePadButton(GamePadButton::BUTTON_LEFT) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_1) == KeyState::KEY_DOWN) EquipGun(0);
    else if (Input::GetGamePadButton(GamePadButton::BUTTON_DOWN) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_2) == KeyState::KEY_DOWN) EquipGun(1);
    else if (Input::GetGamePadButton(GamePadButton::BUTTON_RIGHT) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_3) == KeyState::KEY_DOWN) EquipGun(2);

    if (equipedGun == nullptr) return;

    // press
    if ((Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERRIGHT) > 5000 && canShoot) || Input::GetMouseButton(MouseButton::LEFT) == KeyState::KEY_DOWN)
    {
        equipedGun->Shoot();
        canShoot = false;
    }
    if (Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERRIGHT) < 5000)
    {
        canShoot = true;
    }
}

void PlayerGunManager::SwapGun(bool next)
{
    if (next)
    {
        equipedIndex++;
        if (equipedIndex > 2)
        {
            equipedIndex = 0;
        }
    }
    else
    {
        equipedIndex--;
        if (equipedIndex < 0)
        {
            equipedIndex = 2;
        }
    }

    if (guns[equipedIndex].GetScript("PlayerGunType") == nullptr) SwapGun(next);
}

void PlayerGunManager::EquipNextGun()
{
    if (equipedGun != nullptr) equipedGun->EnableGuns(false);
    SwapGun(true);

    PlayerGunType* gunType = (PlayerGunType*)guns[equipedIndex].GetScript("PlayerGunType");
    switch (gunType->gunType)
    {
    case 0: // duals
        equipedGun = (PlayerGun*)guns[equipedIndex].GetScript("PlayerDuals");
        break;
    case 1: // semiautomatic
        equipedGun = (PlayerGun*)guns[equipedIndex].GetScript("PlayerSemiAuto");
        break;
    default:
        equipedGun = nullptr;
        break;
    }
    if (equipedGun != nullptr) equipedGun->EnableGuns(true);
}

void PlayerGunManager::EquipGun(int index)
{
    PlayerGunType* gunType = (PlayerGunType*)guns[index].GetScript("PlayerGunType");
    if (gunType == nullptr) return;
    
    if (equipedGun != nullptr) equipedGun->EnableGuns(false);
    equipedIndex = index;

    switch (gunType->gunType)
    {
    case 0: // duals
        equipedGun = (PlayerGun*)guns[equipedIndex].GetScript("PlayerDuals");
        break;
    case 1: // semiautomatic
        equipedGun = (PlayerGun*)guns[equipedIndex].GetScript("PlayerSemiAuto");
        break;
    default:
        equipedGun = nullptr;
        break;
    }
    if (equipedGun != nullptr) equipedGun->EnableGuns(true);
}