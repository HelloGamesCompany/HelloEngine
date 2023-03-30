#include "ArmoryUpgratteButtons.h"
HELLO_ENGINE_API_C ArmoryUpgratteButtons* CreateArmoryUpgratteButtons(ScriptToInspectorInterface* script)
{
	ArmoryUpgratteButtons* classInstance = new ArmoryUpgratteButtons();

	script->AddDragBoxUIButton("Unlock", &classInstance->Unlock);
	script->AddDragBoxUIButton("Upgrate 1", &classInstance->Upgrate1);
	script->AddDragBoxUIButton("Upgrate 2", &classInstance->Upgrate2);
	script->AddDragBoxUIButton("Upgrate 3", &classInstance->Upgrate3);

	script->AddDragBoxGameObject("Panel Unlock", &classInstance->PanelUnlock);
	script->AddDragBoxGameObject("Panel Upgrate", &classInstance->PanelUpgrate);

	script->AddDragBoxGameObject("Weapon Associated", &classInstance->currentWeapon);

	return classInstance;
}

void ArmoryUpgratteButtons::Start()
{
	WeaponInstance = (ArmoryWeaponSelect*)currentWeapon.GetScript("ArmoryWeaponSelect");

	isUnlocked = false;
}
void ArmoryUpgratteButtons::Update()
{
	if (Unlock.OnHold())
	{
		//GameObject Arma selected -> GetScript() -> set the correct bool as true
		
		WeaponInstance->PanelUnlock2;
		Console::Log("HOLA?");

		if (WeaponInstance->isUnlockWeapon2)
		{
			Console::Log("Press 2");
		}

		isUnlocked = true;
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