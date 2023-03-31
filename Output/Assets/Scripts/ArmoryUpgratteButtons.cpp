#include "ArmoryUpgratteButtons.h"
HELLO_ENGINE_API_C ArmoryUpgratteButtons* CreateArmoryUpgratteButtons(ScriptToInspectorInterface* script)
{
	ArmoryUpgratteButtons* classInstance = new ArmoryUpgratteButtons();

	script->AddDragBoxUIButton("Unlock", &classInstance->Unlock);
	script->AddDragBoxUIButton("Upgrate 1", &classInstance->Upgrate1);
	script->AddDragBoxUIButton("Upgrate 2", &classInstance->Upgrate2);
	script->AddDragBoxUIButton("Upgrate 3", &classInstance->Upgrate3);

	script->AddDragBoxGameObject("Panel Upgrate W1", &classInstance->PanelUpgrateW1);

	script->AddDragBoxUIInput("Panel Unlock W2", &classInstance->PanelUnlockW2);
	script->AddDragBoxUIInput("Panel Upgrate W2", &classInstance->PanelUpgrateW2);

	script->AddDragBoxGameObject("Panel Unlock W3", &classInstance->PanelUnlockW3);
	script->AddDragBoxGameObject("Panel Upgrate W3", &classInstance->PanelUpgrateW3);

	script->AddDragBoxGameObject("Weapon Associated", &classInstance->currentWeapon);

	script->AddDragBoxUIInput("List Weapons", &classInstance->SelectWeaponList);

	return classInstance;
}

void ArmoryUpgratteButtons::Start()
{
	weaponInstance = (ArmoryWeaponSelect*)currentWeapon.GetScript("ArmoryWeaponSelect");
}
void ArmoryUpgratteButtons::Update()
{
	if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN)
	{
		Console::Log(std::to_string(oneTime));
		SelectWeaponList.SetEnable(true);
		PanelUpgrateW2.SetEnable(false);
		PanelUnlockW2.SetEnable(false);
	}

	if (Unlock.OnPress() && oneTime != 500)
	{
		//GameObject Arma selected -> GetScript() -> set the correct bool as true
		Console::Log("1");
		Console::Log("On Press");
		weaponInstance->isUnlocked = true;
		PanelUpgrateW2.SetEnable(false);
		PanelUnlockW2.SetEnable(false);
		oneTime = 500;
		Console::Log("2");
		/*if (WeaponInstance->isUnlockWeapon2)
		{
			PanelUnlockW2.SetActive(false);
		}

		if (WeaponInstance->isUnlockWeapon3)
		{
			Console::Log("PEPE?");
			PanelUnlockW3.SetActive(false);
		}*/
	}

	if (Upgrate1.OnPress())
	{

	}

	if (Upgrate2.OnPress())
	{

	}

	if (Upgrate3.OnPress())
	{

	}
}