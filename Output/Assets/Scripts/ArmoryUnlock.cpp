#include "ArmoryUnlock.h"
HELLO_ENGINE_API_C ArmoryUnlock* CreateArmoryUnlock(ScriptToInspectorInterface* script)
{
	ArmoryUnlock* classInstance = new ArmoryUnlock();
	script->AddDragBoxUIButton("Unlock", &classInstance->Unlock);
	script->AddDragBoxUIInput("Panel Upgrate", &classInstance->PanelUpgrate);
	script->AddDragBoxUIInput("Panel Unlock", &classInstance->PanelUnlock);
	script->AddDragBoxUIInput("List Weapons", &classInstance->SelectWeaponList);
	script->AddDragBoxGameObject("Weapon Associated", &classInstance->currentWeapon);

	return classInstance;
}

void ArmoryUnlock::Start()
{
	weaponInstance = (ArmoryWeaponSelect*)currentWeapon.GetScript("ArmoryWeaponSelect");
}
void ArmoryUnlock::Update()
{
	if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN)
	{
		SelectWeaponList.SetEnable(true);
		PanelUnlock.SetEnable(false);
	}

	if (Unlock.OnPress() && oneTime != 500)
	{
		//GameObject Arma selected -> GetScript() -> set the correct bool as true.
		Console::Log("Unlock");
		weaponInstance->isUnlocked = true;
		PanelUnlock.SetEnable(false);
		PanelUpgrate.SetEnable(true);
		oneTime = 500;
	}
}