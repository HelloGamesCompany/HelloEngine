#include "ArmoryUnlock.h"
HELLO_ENGINE_API_C ArmoryUnlock* CreateArmoryUnlock(ScriptToInspectorInterface* script)
{
    ArmoryUnlock* classInstance = new ArmoryUnlock();
    script->AddDragBoxUIButton("Unlock", &classInstance->Unlock);
    script->AddDragBoxUIInput("Panel Upgrate", &classInstance->PanelUpgrate);
    script->AddDragBoxUIInput("Panel Unlock", &classInstance->PanelUnlock);
    script->AddDragBoxUIInput("List Weapons", &classInstance->SelectWeaponList);
    script->AddDragBoxGameObject("Weapon Associated", &classInstance->currentWeapon);

    script->AddDragInt("Gun Index", &classInstance->gunIndex);
    return classInstance;
}

void ArmoryUnlock::Start()
{
    weaponInstance = (ArmoryWeaponSelect*)currentWeapon.GetScript("ArmoryWeaponSelect");

    switch (gunIndex)
    {
    case 1:
        haveBlueprint = API_QuickSave::GetBool("semiautomaticGunBlueprint");
        break;
    case 2:
        haveBlueprint = API_QuickSave::GetBool("automaticGunBlueprint");
        break;
    case 3:
        haveBlueprint = API_QuickSave::GetBool("burstGunBlueprint");
        break;
    case 4:
        haveBlueprint = API_QuickSave::GetBool("shotgunGunBlueprint");
        break;
    case 5:
        haveBlueprint = API_QuickSave::GetBool("handgunGunBlueprint");
        break;
    default:
        break;
    }

    Unlock.SetBlocked(!haveBlueprint);
}

void ArmoryUnlock::Update()
{
    if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN && !PanelUnlock.IsEnabled())
    {
        SelectWeaponList.SetEnable(true);
        PanelUnlock.SetEnable(false);
    }
    
    if (Unlock.OnPress() && weaponInstance->unlockPress != 1 && haveBlueprint)
    {
        switch (gunIndex)
        {
        case 1:
            API_QuickSave::SetBool("semiautomaticUnlocked", true);
            break;
        case 2:
            API_QuickSave::SetBool("automaticUnlocked", true);
            break;
        case 3:
            API_QuickSave::SetBool("burstUnlocked", true);
            break;
        case 4:
            API_QuickSave::SetBool("shotgunUnlocked", true);
            break;
        case 5:
            API_QuickSave::SetBool("handgunUnlocked", true);
            break;
        default:
            break;
        }

        //Console::Log(std::to_string(weaponInstance->unlockPress++));
        weaponInstance->isUnlocked = true;
        PanelUnlock.SetEnable(false);
        PanelUpgrate.SetEnable(true);
        weaponInstance->unlockPress++;
    }
}