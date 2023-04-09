#include "ArmoryWeaponSelect.h"
HELLO_ENGINE_API_C ArmoryWeaponSelect* CreateArmoryWeaponSelect(ScriptToInspectorInterface* script)
{
    ArmoryWeaponSelect* classInstance = new ArmoryWeaponSelect();

    script->AddDragBoxUIButton("Weapon", &classInstance->CurrentWeapon);

    script->AddDragBoxGameObject("Next Weapon", &classInstance->NextWeapon);

    script->AddDragBoxGameObject("Prev Weapon", &classInstance->PrevtWeapon);

    script->AddDragBoxUIInput("Panel Upgrate", &classInstance->CurrentPanelUpgrate);
    script->AddDragBoxUIInput("Panel Unlock", &classInstance->CurrentPanelUnlock);

    script->AddDragBoxGameObject("Next Panel Upgrate", &classInstance->NextPanelUpgrate);
    script->AddDragBoxGameObject("Next Panel Unlock", &classInstance->NextPanelUnlock);

    script->AddDragBoxGameObject("Prev Panel Upgrate", &classInstance->PrevPanelUpgrate);
    script->AddDragBoxGameObject("Prev Panel Unlock", &classInstance->PrevPanelUnlock);

    script->AddDragBoxTextureResource("Material Weapon", &classInstance->CurrentTextureWeapon);
    script->AddDragBoxTextureResource("Material Weapon Locked", &classInstance->CurrentTextureWeaponLocked);

    script->AddDragBoxMaterialComponent("SelectedWeapon", &classInstance->SelectedWeapon);

    script->AddDragBoxUIInput("List Weapons", &classInstance->SelectWeaponList);

    script->AddDragInt("Gun Index", &classInstance->gunIndex);

    return classInstance;
}

void ArmoryWeaponSelect::Start()
{
    nextW = (ArmoryWeaponSelect*)NextWeapon.GetScript("ArmoryWeaponSelect");
    PrevW = (ArmoryWeaponSelect*)PrevtWeapon.GetScript("ArmoryWeaponSelect");
    CurrentPanelUnlock.SetEnable(false);

    FindUnlock();
}
void ArmoryWeaponSelect::Update()
{
    if (CurrentWeapon.OnHovered())
    {
        if (findUnlock) FindUnlock();

        if (isUnlocked)
        {
            SelectedWeapon.ChangeAlbedoTexture(CurrentTextureWeapon);
            CurrentPanelUpgrate.GetGameObject().SetActive(true);
            CurrentPanelUnlock.GetGameObject().SetActive(false);
            //CurrentWeapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(CurrentTextureWeapon);
        }
        else
        {
            SelectedWeapon.ChangeAlbedoTexture(CurrentTextureWeaponLocked);
            CurrentPanelUnlock.GetGameObject().SetActive(true);
        }

        if (nextW != nullptr && nextW->isUnlocked)
            NextPanelUpgrate.SetActive(false);
        else if (nextW != nullptr && !nextW->isUnlocked)
            NextPanelUnlock.SetActive(false);

        if (PrevW != nullptr && PrevW->isUnlocked)
            PrevPanelUpgrate.SetActive(false);
        else if (PrevW != nullptr && !PrevW->isUnlocked)
            PrevPanelUnlock.SetActive(false);
    }

    if (CurrentWeapon.OnPress())
    {
        SelectWeaponList.SetEnable(false);
        if (isUnlocked)
        {
            CurrentPanelUpgrate.SetEnable(true);
        }
        else
        {
            CurrentPanelUnlock.SetEnable(true);
            unlockPress++;
        }
    }
}

void ArmoryWeaponSelect::FindUnlock()
{
    if (gunIndex == 0) isUnlocked = true;
    else
    {
        switch (gunIndex)
        {
        case 1:
            isUnlocked = API_QuickSave::GetBool("semiautomaticUnlocked");
            break;
        case 2:
            isUnlocked = API_QuickSave::GetBool("automaticUnlocked");
            break;
        case 3:
            isUnlocked = API_QuickSave::GetBool("burstUnlocked");
            break;
        case 4:
            isUnlocked = API_QuickSave::GetBool("shotgunUnlocked");
            break;
        case 5:
            isUnlocked = API_QuickSave::GetBool("handgunUnlocked");
            break;
        default:
            break;
        }
    }

    findUnlock = false;
}