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

	script->AddDragBoxMaterialComponent("SelectedWeapon", &classInstance->SelectedWeapon);

	script->AddDragBoxUIInput("List Weapons", &classInstance->SelectWeaponList);

	return classInstance;
}

void ArmoryWeaponSelect::Start()
{
	nextW = (ArmoryWeaponSelect*)NextWeapon.GetScript("ArmoryWeaponSelect");
	PrevW = (ArmoryWeaponSelect*)PrevtWeapon.GetScript("ArmoryWeaponSelect");
}
void ArmoryWeaponSelect::Update()
{
	if (CurrentWeapon.OnHovered())
	{
		SelectedWeapon.ChangeAlbedoTexture(CurrentTextureWeapon);

		if (isUnlocked)
		{
			CurrentPanelUpgrate.GetGameObject().SetActive(true);
			CurrentWeapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(CurrentTextureWeapon);
		}
		else
			CurrentPanelUnlock.GetGameObject().SetActive(true);

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

void ArmoryWeaponSelect::UnlockWeapon2()
{

}