#include "ArmoryUpgratteButtons.h"
HELLO_ENGINE_API_C ArmoryUpgratteButtons* CreateArmoryUpgratteButtons(ScriptToInspectorInterface* script)
{
    ArmoryUpgratteButtons* classInstance = new ArmoryUpgratteButtons();

    script->AddDragBoxUIButton("Upgrate 1", &classInstance->Upgrate1);
    script->AddDragBoxUIButton("Upgrate 2", &classInstance->Upgrate2);
    script->AddDragBoxUIButton("Upgrate 3", &classInstance->Upgrate3);

    script->AddDragBoxUIInput("Panel Upgrate", &classInstance->PanelUpgrate);

    script->AddDragBoxGameObject("Weapon Associated", &classInstance->currentWeapon);
    script->AddDragBoxGameObject("Player", &classInstance->Player);

    script->AddDragBoxUIInput("List Weapons", &classInstance->SelectWeaponList);

    script->AddDragBoxUIText("Text Blueprints", &classInstance->bluePrintText);

    script->AddDragInt("Gun Index", &classInstance->gunIndex);

    return classInstance;
}

void ArmoryUpgratteButtons::Start()
{
    switch (gunIndex)
    {
    case 1:
        gunLevel = API_QuickSave::GetBool("semiautomatic_level");
        break;
    case 2:
        gunLevel = API_QuickSave::GetBool("automatic_level");
        break;
    case 3:
        gunLevel = API_QuickSave::GetBool("burst_level");
        break;
    case 4:
        gunLevel = API_QuickSave::GetBool("shotgun_level");
        break;
    case 5:
        gunLevel = API_QuickSave::GetBool("handgun_level");
        break;
    default:
        break;
    }

    _playerStorage = (PlayerStorage*)Player.GetScript("PlayerStorage");
    if (_playerStorage == nullptr) Console::Log("Player Storage missing in ArmaryUpgradeButtons.");

    if (gunLevel > 0) Upgrate1.SetBlocked(true);
    if (gunLevel > 1) Upgrate2.SetBlocked(true);
    if (gunLevel > 2) Upgrate3.SetBlocked(true);
}

void ArmoryUpgratteButtons::Update()
{
    if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN)
    {
        SelectWeaponList.SetEnable(true);
        PanelUpgrate.SetEnable(false);
    }

    if (!_playerStorage) return;

    if (Upgrate1.OnPress() && _playerStorage->upgradeBlueprintAmount > 0)
    {
        UpgradeGun();
        Upgrate2.SetBlocked(false);
        if (_playerStorage != nullptr)
        {
            bluePrintText.SetText(std::to_string(_playerStorage->upgradeBlueprintAmount).c_str());
        }
    }

    if (Upgrate2.OnPress() && _playerStorage->upgradeBlueprintAmount > 0)
    {
        UpgradeGun();
        Upgrate3.SetBlocked(false);
        if (_playerStorage != nullptr)
        {
            bluePrintText.SetText(std::to_string(_playerStorage->upgradeBlueprintAmount).c_str());
        }
    }

    if (Upgrate3.OnPress() && _playerStorage->upgradeBlueprintAmount > 0)
    {
        UpgradeGun();
        if (_playerStorage != nullptr)
        {
            bluePrintText.SetText(std::to_string(_playerStorage->upgradeBlueprintAmount).c_str());
        }
    }
}

void ArmoryUpgratteButtons::UpgradeGun()
{
    _playerStorage->upgradeBlueprintAmount--;
    _playerStorage->SaveData();

    gunLevel++;

    switch (gunIndex)
    {
    case 1:
        API_QuickSave::SetBool("semiautomatic_level", gunLevel);
        break;
    case 2:
        API_QuickSave::SetBool("automatic_level", gunLevel);
        break;
    case 3:
        API_QuickSave::SetBool("burst_level", gunLevel);
        break;
    case 4:
        API_QuickSave::SetBool("shotgun_level", gunLevel);
        break;
    case 5:
        API_QuickSave::SetBool("handgun_level", gunLevel);
        break;
    default:
        break;
    }
}