#include "ArmoryUpgratteButtons.h"
HELLO_ENGINE_API_C ArmoryUpgratteButtons* CreateArmoryUpgratteButtons(ScriptToInspectorInterface* script)
{
	ArmoryUpgratteButtons* classInstance = new ArmoryUpgratteButtons();

	script->AddDragBoxUIButton("Unlock", &classInstance->Unlock);
	script->AddDragBoxUIButton("Upgrate 1", &classInstance->Upgrate1);
	script->AddDragBoxUIButton("Upgrate 2", &classInstance->Upgrate2);
	script->AddDragBoxUIButton("Upgrate 3", &classInstance->Upgrate3);

	script->AddDragBoxUIInput("Panel Unlock", &classInstance->PanelUnlock);
	script->AddDragBoxUIInput("Panel Upgrate", &classInstance->PanelUpgrate);

	script->AddDragBoxGameObject("Weapon Associated", &classInstance->currentWeapon);

	script->AddDragBoxUIInput("List Weapons", &classInstance->SelectWeaponList);

	script->AddDragBoxUIText("Text Blueprints", &classInstance->bluePrintText);

	return classInstance;
}

void ArmoryUpgratteButtons::Start()
{
	weaponInstance = (ArmoryWeaponSelect*)currentWeapon.GetScript("ArmoryWeaponSelect");
	upgrade1 = false;
	upgrade2 = false;
}
void ArmoryUpgratteButtons::Update()
{
	if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN)
	{
		//Console::Log(std::to_string(oneTime));
		SelectWeaponList.SetEnable(true);
		PanelUpgrate.SetEnable(false);
		PanelUnlock.SetEnable(false);
	}

	if (Unlock.OnPress() && oneTime != 500)
	{
		//GameObject Arma selected -> GetScript() -> set the correct bool as true
		weaponInstance->isUnlocked = true;
		PanelUpgrate.SetEnable(false);
		PanelUnlock.SetEnable(false);
		oneTime = 500;
	}

	if (Upgrate1.OnPress())
	{
		Upgrade1();
		bluePrintText.SetText("HOLA");
		upgrade1 = true;
	}

	if (Upgrate2.OnPress() && upgrade1)
	{
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

}

void ArmoryUpgratteButtons::Upgrade2()
{

}

void ArmoryUpgratteButtons::Upgrade3()
{

}