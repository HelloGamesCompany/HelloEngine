#include "ArmoryWeaponSelect.h"
HELLO_ENGINE_API_C ArmoryWeaponSelect* CreateArmoryWeaponSelect(ScriptToInspectorInterface* script)
{
	ArmoryWeaponSelect* classInstance = new ArmoryWeaponSelect();

	script->AddDragBoxUIButton("Weapon", &classInstance->CurrentWeapon);

	script->AddDragBoxGameObject("Next Weapon", &classInstance->NextWeapon);

	script->AddDragBoxGameObject("Prev Weapon", &classInstance->PrevtWeapon);

	script->AddDragBoxGameObject("Panel Upgrate", &classInstance->CurrentPanelUpgrate);
	script->AddDragBoxGameObject("Panel Unlock", &classInstance->CurrentPanelUnlock);

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
			CurrentPanelUpgrate.SetActive(true);
		else
			CurrentPanelUnlock.SetActive(true);

		if (nextW->isUnlocked && nextW != nullptr)
			NextPanelUpgrate.SetActive(false);
		else if (!nextW->isUnlocked && nextW != nullptr)
			NextPanelUnlock.SetActive(false);

		if (PrevW->isUnlocked && PrevW != nullptr)
			PrevPanelUpgrate.SetActive(false);
		else if (!PrevW->isUnlocked && PrevW != nullptr)
			PrevPanelUnlock.SetActive(false);
	}

	if (CurrentWeapon.OnPress())
	{
		SelectWeaponList.SetEnable(false);
		CurrentPanelUnlock.SetActive(false);
		isUnlocked = true;
	}
}

void ArmoryWeaponSelect::UnlockWeapon2()
{

}