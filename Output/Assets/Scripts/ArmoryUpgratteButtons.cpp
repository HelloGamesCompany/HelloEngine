#include "ArmoryUpgratteButtons.h"
HELLO_ENGINE_API_C ArmoryUpgratteButtons* CreateArmoryUpgratteButtons(ScriptToInspectorInterface* script)
{
	ArmoryUpgratteButtons* classInstance = new ArmoryUpgratteButtons();

	script->AddDragBoxUIButton("Unlock", &classInstance->Unlock);
	script->AddDragBoxUIButton("Upgrate 1", &classInstance->Upgrate1);
	script->AddDragBoxUIButton("Upgrate 2", &classInstance->Upgrate2);
	script->AddDragBoxUIButton("Upgrate 3", &classInstance->Upgrate3);

	script->AddDragBoxGameObject("Panel Upgrate W1", &classInstance->PanelUpgrateW1);

	script->AddDragBoxGameObject("Panel Unlock W2", &classInstance->PanelUnlockW2);
	script->AddDragBoxGameObject("Panel Upgrate W2", &classInstance->PanelUpgrateW2);

	script->AddDragBoxGameObject("Panel Unlock W3", &classInstance->PanelUnlockW3);
	script->AddDragBoxGameObject("Panel Upgrate W3", &classInstance->PanelUpgrateW3);

	script->AddDragBoxGameObject("Weapon Associated", &classInstance->currentWeapon);

	return classInstance;
}

void ArmoryUpgratteButtons::Start()
{
	//WeaponInstance = (ArmoryWeaponSelect*)currentWeapon.GetScript("ArmoryWeaponSelect");
	Console::Log("Start");
	isUnlocked = false;
}
void ArmoryUpgratteButtons::Update()
{
	if (Unlock.OnPress())
	{
		//GameObject Arma selected -> GetScript() -> set the correct bool as true
		Console::Log("SI?");

		/*if (WeaponInstance->isUnlockWeapon2)
		{
			PanelUnlockW2.SetActive(false);
		}

		if (WeaponInstance->isUnlockWeapon3)
		{
			Console::Log("PEPE?");
			PanelUnlockW3.SetActive(false);
		}*/

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