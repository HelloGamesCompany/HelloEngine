#include "ArmoryUnlock.h"
HELLO_ENGINE_API_C ArmoryUnlock* CreateArmoryUnlock(ScriptToInspectorInterface* script)
{
    ArmoryUnlock* classInstance = new ArmoryUnlock();
    script->AddDragBoxUIButton("Unlock", &classInstance->Unlock);
    script->AddDragBoxUIInput("Panel Unlock", &classInstance->PanelUnlock);
    script->AddDragBoxUIInput("List Weapons", &classInstance->SelectWeaponList);

    script->AddDragInt("Gun Index", &classInstance->gunIndex);
    return classInstance;
}

void ArmoryUnlock::Start()
{
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

    //Unlock.SetBlocked(!haveBlueprint);
}

void ArmoryUnlock::Update()
{
    if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN && PanelUnlock.IsEnabled())
    {
        SelectWeaponList.SetEnable(true);
        PanelUnlock.SetEnable(false);
        Console::Log("BACK");
    }

    if (manteinTime > 0.0f)
    {
        manteinTime -= Time::GetRealTimeDeltaTime();

        if (Unlock.OnRelease())
        {
            manteinTime = 0.0f;
        }
        else if (manteinTime <= 0.0f)
        {
            manteinTime = 0.0f;
            
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

            PanelUnlock.SetEnable(false);
            SelectWeaponList.SetEnable(true);
        }
    }
    
    if (Unlock.OnPress() && manteinTime == 0.0f/* && haveBlueprint*/)
    {
        manteinTime = 1.0f;
    }
}