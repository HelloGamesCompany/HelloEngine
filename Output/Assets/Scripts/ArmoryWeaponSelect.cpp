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


	return classInstance;
}

void ArmoryWeaponSelect::Start()
{
	nextW = (ArmoryWeaponSelect*)NextWeapon.GetScript("ArmoryUpgratteButtons");
	PrevW = (ArmoryWeaponSelect*)PrevtWeapon.GetScript("ArmoryUpgratteButtons");

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

		if (nextW->isUnlocked)
			NextPanelUpgrate.SetActive(true);
		else
			NextPanelUnlock.SetActive(true);

		if (PrevW->isUnlocked)
			PrevPanelUpgrate.SetActive(true);
		else
			PrevPanelUnlock.SetActive(true);
	}

	if (CurrentWeapon.OnPress())
	{
		CurrentPanelUnlock.SetActive(false);
		isUnlocked = true;
	}

	/*if (Weapon1.OnHovered())
	{
		SelectedWeapon.ChangeAlbedoTexture(Weapon1Texture);

		if (isUnlockWeapon2)
			PanelUpgrate2.SetActive(false);
		else
			PanelUnlock2.SetActive(false);

		PanelUpgrate1.SetActive(true);
	}

	if (Weapon2.OnHovered())
	{
		SelectedWeapon.ChangeAlbedoTexture(Weapon2Texture);

		if (isUnlockWeapon3)
			PanelUpgrate3.SetActive(false);
		else
			PanelUnlock3.SetActive(false);

		PanelUpgrate1.SetActive(false);

		if (isUnlockWeapon2)
			PanelUpgrate2.SetActive(true);
		else
			PanelUnlock2.SetActive(true);
	}

	if (Weapon3.OnHovered())
	{
		SelectedWeapon.ChangeAlbedoTexture(Weapon3Texture);

		if (isUnlockWeapon2)
			PanelUpgrate2.SetActive(false);
		else
			PanelUnlock2.SetActive(false);

		if (isUnlockWeapon4)
			PanelUpgrate4.SetActive(false);
		else
			PanelUnlock4.SetActive(false);

		if (isUnlockWeapon3)
			PanelUpgrate3.SetActive(true);
		else
			PanelUnlock3.SetActive(true);
	}

	if (Weapon4.OnHovered())
	{
		SelectedWeapon.ChangeAlbedoTexture(Weapon4Texture);

		if (isUnlockWeapon3)
			PanelUpgrate3.SetActive(false);
		else
			PanelUnlock3.SetActive(false);

		if (isUnlockWeapon5)
			PanelUpgrate5.SetActive(false);
		else
			PanelUnlock5.SetActive(false);

		if (isUnlockWeapon4)
			PanelUpgrate4.SetActive(true);
		else
			PanelUnlock4.SetActive(true);
	}

	if (Weapon5.OnHovered())
	{
		SelectedWeapon.ChangeAlbedoTexture(Weapon5Texture);

		if (isUnlockWeapon4)
			PanelUpgrate4.SetActive(false);
		else
			PanelUnlock4.SetActive(false);

		if (isUnlockWeapon6)
			PanelUpgrate6.SetActive(false);
		else
			PanelUnlock6.SetActive(false);

		if (isUnlockWeapon5)
			PanelUpgrate5.SetActive(true);
		else
			PanelUnlock5.SetActive(true);
	}

	if (Weapon6.OnHovered())
	{
		SelectedWeapon.ChangeAlbedoTexture(Weapon6Texture);

		if (isUnlockWeapon5)
			PanelUpgrate5.SetActive(false);
		else
			PanelUnlock5.SetActive(false);

		if (isUnlockWeapon6)
			PanelUpgrate6.SetActive(true);
		else
			PanelUnlock6.SetActive(true);
	}

	if (Weapon1.OnPress())
	{

	}

	if (Weapon2.OnPress())
	{
		//desactivar componente
		isUnlockWeapon2 = true;
	}

	if (Weapon3.OnPress())
	{
		Console::Log("JUAN?");

		isUnlockWeapon3 = true;
	}

	if (Weapon4.OnPress())
	{
		//isUnlockWeapon4 = true;
	}

	if (Weapon5.OnPress())
	{
		//isUnlockWeapon5 = true;
	}

	if (Weapon6.OnPress())
	{
		//isUnlockWeapon6 = true;
	}*/
}

void ArmoryWeaponSelect::UnlockWeapon2()
{

}