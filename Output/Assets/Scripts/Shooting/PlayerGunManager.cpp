#include "PlayerGunManager.h"
#include "../UI Test folder/SwapWeapon.h"
#include "../Player/PlayerMove.h"

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
    script->AddDragBoxGameObject("HUD", &classInstance->HUDGameObject);
    script->AddDragBoxGameObject("PlayerMovement GameObject", &classInstance->playerMoveGameObject);

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
    EquipGun(0);
    HUDScript = (SwapWeapon*)HUDGameObject.GetScript("SwapWeapon");
    playerMovementScript = (PlayerMove*)playerMoveGameObject.GetScript("PlayerMove");
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
            HUDScript->SwapWeapon3();
        }
        else bufferLB = 0.1f;
    }
    if (Input::GetGamePadButton(GamePadButton::BUTTON_RIGHT_SHOULDER) == KeyState::KEY_DOWN)
    {
        if (bufferLB > 0.0f)
        {
            EquipGun(gunOnHandIndex3); // special weapon
            bufferLB = 0.0f;
            HUDScript->SwapWeapon3();
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
            HUDScript->SwapWeapon1();
        }
    }
    if (bufferRB > 0.0f)
    {
        bufferRB -= Time::GetDeltaTime();
        if (bufferRB <= 0)
        {
            EquipGun(gunOnHandIndex2); // normal weapon
            bufferRB = 0.0f;
            HUDScript->SwapWeapon2();
        }
    }

    if (equipedGun == nullptr) return;

    if (Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERRIGHT) > 5000 || Input::GetMouseButton(MouseButton::LEFT) == KeyState::KEY_REPEAT)
    {
        if (playerStats->GetAmmonByType(equipedGun->ammoType) > 0)
        {
            equipedGun->Shoot();
            playerMovementScript->ShootAnim();
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
    case 7: // ricochet
        equipedGun = (PlayerGun*)guns[equipedIndex].GetScript("PlayerRicochet");
        break;
    default:
        equipedGun = nullptr;
        break;
    }
    if (equipedGun != nullptr) equipedGun->EnableGuns(true);
}