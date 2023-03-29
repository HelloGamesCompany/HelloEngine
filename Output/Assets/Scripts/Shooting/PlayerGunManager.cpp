#include "PlayerGunManager.h"
#include "../UI Test folder/SwapWeapon.h"
HELLO_ENGINE_API_C PlayerGunManager* CreatePlayerGunManager(ScriptToInspectorInterface* script)
{
    PlayerGunManager* classInstance = new PlayerGunManager();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->player);
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
    script->AddDragBoxGameObject("Ricochet", &classInstance->ricochet);
    script->AddDragFloat("Swap Delay", &classInstance->maxSwapDelay);
    script->AddDragFloat("Fast Swap Delay", &classInstance->maxFastSwapDelay);
    script->AddDragBoxGameObject("WeaponUI", &classInstance->weaponUI);
    return classInstance;
}

void PlayerGunManager::Start()
{
    playerStats = (PlayerStats*)player.GetScript("PlayerStats");

    // add guns to the array in order
    guns.push_back(duals);
    guns.push_back(semiauto);
    guns.push_back(automatic);
    guns.push_back(burst);
    guns.push_back(shotgun);
    guns.push_back(handgun);
    guns.push_back(flamethrower);
    guns.push_back(ricochet);

    // get start guns
    GetGun(1, gunOnHandIndex1);
    GetGun(2, gunOnHandIndex2);
    GetGun(3, gunOnHandIndex3);

    // start with base gun selected
    UnequipGun(0);
}

void PlayerGunManager::Update()
{
    float dt;
    if (playerStats->slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    // Keyboard
    if (Input::GetKey(KeyCode::KEY_1) == KeyState::KEY_DOWN) { UnequipGun(gunOnHandIndex1); if (weaponUI.IsAlive() == true) { ((SwapWeapon*)weaponUI.GetScript("SwapWeapon"))->SwapWeapon1(); }  }
    else if (Input::GetKey(KeyCode::KEY_2) == KeyState::KEY_DOWN) { UnequipGun(gunOnHandIndex2); if (weaponUI.IsAlive() == true) { ((SwapWeapon*)weaponUI.GetScript("SwapWeapon"))->SwapWeapon2(); } }
    else if (Input::GetKey(KeyCode::KEY_3) == KeyState::KEY_DOWN) { UnequipGun(gunOnHandIndex3); if (weaponUI.IsAlive() == true) { ((SwapWeapon*)weaponUI.GetScript("SwapWeapon"))->SwapWeapon3(); } }

    // gamepad
    if (Input::GetGamePadButton(GamePadButton::BUTTON_LEFT_SHOULDER) == KeyState::KEY_DOWN)
    {
        if (bufferRB > 0)
        {
            UnequipGun(gunOnHandIndex3); // special weapon
            bufferRB = 0.0f;
        }
        else bufferLB = 0.1f;
    }
    if (Input::GetGamePadButton(GamePadButton::BUTTON_RIGHT_SHOULDER) == KeyState::KEY_DOWN)
    {
        if (bufferLB > 0.0f)
        {
            UnequipGun(gunOnHandIndex3); // special weapon
            bufferLB = 0.0f;
        }
        else bufferRB = 0.1f;
    }
    if (bufferLB > 0.0f)
    {
        bufferLB -= Time::GetRealTimeDeltaTime();
        if (bufferLB <= 0.0f)
        {
            UnequipGun(gunOnHandIndex1); // base weapon
            bufferLB = 0.0f;
        }
    }
    if (bufferRB > 0.0f)
    {
        bufferRB -= Time::GetRealTimeDeltaTime();
        if (bufferRB <= 0)
        {
            UnequipGun(gunOnHandIndex2); // normal weapon
            bufferRB = 0.0f;
        }
    }

    // swap gun
    if (swapDelay > 0.0f)
    {
        swapDelay -= dt;
        if (swapDelay <= 0.0f)
        {
            swapDelay = 0.0f;
            EquipGun(swapToIndex);
        }
        return;
    }

    if (equipedGun == nullptr) return;

    if (Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERRIGHT) > 5000 || Input::GetMouseButton(MouseButton::LEFT) == KeyState::KEY_REPEAT)
    {
        if (playerStats->GetAmmonByType(equipedGun->ammoType) > 0)
        {
            equipedGun->Shoot();
        }
        else
        {
            // no ammo sound?
        }
    }

    //switch (equipedIndex)
    //{
    //case 0: // press and release
    //case 1:
    //case 4:
    //case 5:
    //case 7:
    //    if ((Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERRIGHT) > 5000 && canShoot) || Input::GetMouseButton(MouseButton::LEFT) == KeyState::KEY_DOWN)
    //    {
    //        if (playerStats->GetAmmonByType(equipedGun->ammoType) > 0)
    //        {
    //            equipedGun->Shoot();
    //            canShoot = false;
    //        }
    //        else
    //        {
    //            // no ammo sound?
    //        }
    //    }
    //    if (Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERRIGHT) < 5000)
    //    {
    //        canShoot = true;
    //    }
    //    break;
    //case 2: // mantein pressed
    //case 3:
    //case 6:
    //    if (Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERRIGHT) > 5000 || Input::GetMouseButton(MouseButton::LEFT) == KeyState::KEY_REPEAT)
    //    {
    //        if (playerStats->GetAmmonByType(equipedGun->ammoType) > 0)
    //        {
    //            equipedGun->Shoot();
    //        }
    //        else
    //        {
    //            // no ammo sound?
    //        }
    //    }
    //    break;
    //default:
    //    break;
    //}
    
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
        Console::Log("Invalid slot, slot should be 1, 2 or 3.");
        break;
    }

    UnequipGun(gunIndex);
}

void PlayerGunManager::EquipGun(int index)
{
    PlayerGunType* gunType = (PlayerGunType*)guns[index].GetScript("PlayerGunType");
    if (gunType == nullptr) return;

    switch (gunType->gunType)
    {
    case 0: // duals
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerDuals");
        break;
    case 1: // semiautomatic
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerSemiAuto");
        break;
    case 2: // automatic
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerAutomatic");
        break;
    case 3: // burst
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerBurst");
        break;
    case 4: // shotgun
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerShotgun");
        break;
    case 5: // handgun
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerSemiAuto");
        break;
    case 6: // flamethrower
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerFlamethrower");
        break;
    case 7: // ricochet
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerRicochet");
        break;
    default:
        equipedGun = nullptr;
        break;
    }
    if (equipedGun != nullptr) equipedGun->EnableGuns(true);
}

void PlayerGunManager::UnequipGun(int index)
{
    if (equipedGun != nullptr) equipedGun->EnableGuns(false);
    equipedIndex = index;

    if (playerStats->armoryTreeLvl > 0) swapDelay = maxFastSwapDelay + 0.001f;
    else swapDelay = maxSwapDelay + 0.001f;
    swapToIndex = index;
}