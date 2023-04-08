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

	return classInstance;
}

void ArmoryUpgratteButtons::Start()
{
	/*weaponInstance = (ArmoryWeaponSelect*)currentWeapon.GetScript("ArmoryWeaponSelect");
	if (weaponInstance == nullptr)
	{
		Console::Log("Armor Weapon Select is null pointer in the Associated weapon");
	}*/
	_playerStorage = (PlayerStorage*)Player.GetScript("PlayerStorage");
	if (_playerStorage == nullptr)
	{
		Console::Log("Player is null pointer");
	}
	upgrade1 = false;
	upgrade2 = false;

	//bluePrintText.GetGameObject().GetMeshRenderer()
}
void ArmoryUpgratteButtons::Update()
{
	if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN)
	{
		SelectWeaponList.SetEnable(true);
		PanelUpgrate.SetEnable(false);
	}

	if (Upgrate1.OnPress())
	{
		Upgrade1();
		Upgrate2.SetBlocked(false);
		Console::Log("Pepep");
		if (_playerStorage != nullptr)
		{
			bluePrintText.SetText(std::to_string(_playerStorage->upgradeBlueprintAmount).c_str());
		}
		upgrade1 = true;
	}

	if (Upgrate2.OnPress() && upgrade1)
	{
		Upgrate3.SetBlocked(false);
		Upgrade2();
		upgrade2 = true;
	}

	if (Upgrate3.OnPress() && upgrade2)
	{
		Upgrade3();
	}
}

void ArmoryUpgratteButtons::Upgrade1()
{
	Console::Log("1");
}

void ArmoryUpgratteButtons::Upgrade2()
{
	Console::Log("2");

}

void ArmoryUpgratteButtons::Upgrade3()
{
	Console::Log("3");
}