#include "PlayerGunManager.h"
HELLO_ENGINE_API_C PlayerGunManager* CreatePlayerGunManager(ScriptToInspectorInterface* script)
{
    PlayerGunManager* classInstance = new PlayerGunManager();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragInt("Base Gun", &classInstance->gunOnHandIndex1);
    script->AddDragInt("Normal Gun", &classInstance->gunOnHandIndex2);
    script->AddDragInt("Special Gun", &classInstance->gunOnHandIndex3);
    script->AddDragBoxGameObject("Duals", &classInstance->duals);
    script->AddDragBoxGameObject("Semiautomatic", &classInstance->semiauto);
    script->AddDragBoxGameObject("Automatic", &classInstance->automatic);
    script->AddDragBoxGameObject("Burst", &classInstance->burst);
    script->AddDragBoxGameObject("Shotgun", &classInstance->shotgun);
    script->AddDragBoxGameObject("Handgun", &classInstance->handgun);
    script->AddDragBoxGameObject("Flamethrower", &classInstance->flamethrower);
    return classInstance;
}

void PlayerGunManager::Start()
{
    // add guns to the array in order
    guns.push_back(duals);
    guns.push_back(semiauto);
    guns.push_back(automatic);
    guns.push_back(burst);
    guns.push_back(shotgun);
    guns.push_back(handgun);
    guns.push_back(flamethrower);

    // get start guns
    GetGun(1, gunOnHandIndex1);
    GetGun(2, gunOnHandIndex2);
    GetGun(3, gunOnHandIndex3);

    // start with base gun selected
    EquipGun(0);
}

void PlayerGunManager::Update()
{
    // Keyboard
    if (Input::GetKey(KeyCode::KEY_1) == KeyState::KEY_DOWN) EquipGun(gunOnHandIndex1);
    else if (Input::GetKey(KeyCode::KEY_2) == KeyState::KEY_DOWN) EquipGun(gunOnHandIndex2);
    else if (Input::GetKey(KeyCode::KEY_3) == KeyState::KEY_DOWN) EquipGun(gunOnHandIndex3);

    // gamepad
    if (Input::GetGamePadButton(GamePadButton::BUTTON_LEFT_SHOULDER) == KeyState::KEY_DOWN)
    {
        if (bufferRB > 0)
        {
            EquipGun(gunOnHandIndex3); // special weapon
            bufferRB = 0.0f;
        }
        else bufferLB = 0.1f;
    }
    if (Input::GetGamePadButton(GamePadButton::BUTTON_RIGHT_SHOULDER) == KeyState::KEY_DOWN)
    {
        if (bufferLB > 0.0f)
        {
            EquipGun(gunOnHandIndex3); // special weapon
            bufferLB = 0.0f;
        }
        else bufferRB = 0.1f;
    }
    if (bufferLB > 0.0f)
    {
        bufferLB -= Time::GetDeltaTime();
        if (bufferLB <= 0.0f)
        {
            EquipGun(gunOnHandIndex1); // base weapon
            bufferLB = 0.0f;
        }
    }
    if (bufferRB > 0.0f)
    {
        bufferRB -= Time::GetDeltaTime();
        if (bufferRB <= 0)
        {
            EquipGun(gunOnHandIndex2); // normal weapon
            bufferRB = 0.0f;
        }
    }

    if (equipedGun == nullptr) return;

    switch (equipedIndex)
    {
    case 0:
    case 1:
    case 4: // press and release
    case 5:
        if ((Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERRIGHT) > 5000 && canShoot) || Input::GetMouseButton(MouseButton::LEFT) == KeyState::KEY_DOWN)
        {
            equipedGun->Shoot();
            canShoot = false;
        }
        if (Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERRIGHT) < 5000)
        {
            canShoot = true;
        }
        break;
    case 2: // mantein pressed
    case 3:
    case 6:
        if (Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERRIGHT) > 5000 || Input::GetMouseButton(MouseButton::LEFT) == KeyState::KEY_REPEAT)
        {
            equipedGun->Shoot();
        }
        break;
    default:
        break;
    }
    
}

void PlayerGunManager::GetGun(int slot, int gunIndex)
{
    switch (slot)
    {
    case 1:
        gunOnHandIndex1 = gunIndex;
        break;
    case 2:
        gunOnHandIndex2 = gunIndex;
        break;
    case 3:
        gunOnHandIndex3 = gunIndex;
        break;
    default:
        Console::Log("Invalid slot, slot should be between 1, 2 or 3.");
        break;
    }

    EquipGun(gunIndex);
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
    case 2: // automatic
        equipedGun = (PlayerGun*)guns[equipedIndex].GetScript("PlayerAutomatic");
        break;
    case 3: // burst
        equipedGun = (PlayerGun*)guns[equipedIndex].GetScript("PlayerBurst");
        break;
    case 4: // shotgun
        equipedGun = (PlayerGun*)guns[equipedIndex].GetScript("PlayerShotgun");
        break;
    case 5: // handgun
        equipedGun = (PlayerGun*)guns[equipedIndex].GetScript("PlayerSemiAuto");
        break;
    case 6: // flamethrower
        equipedGun = (PlayerGun*)guns[equipedIndex].GetScript("PlayerFlamethrower");
        break;
    default:
        equipedGun = nullptr;
        break;
    }
    if (equipedGun != nullptr) equipedGun->EnableGuns(true);
}