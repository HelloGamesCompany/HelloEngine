#include "ArmoryWeaponSelect.h"
HELLO_ENGINE_API_C ArmoryWeaponSelect* CreateArmoryWeaponSelect(ScriptToInspectorInterface* script)
{
	ArmoryWeaponSelect* classInstance = new ArmoryWeaponSelect();

	script->AddDragBoxUIButton("Weapon5", &classInstance->Weapon5);
	script->AddDragBoxUIButton("Weapon1", &classInstance->Weapon1);
	script->AddDragBoxUIButton("Weapon2", &classInstance->Weapon2);
	script->AddDragBoxUIButton("Weapon3", &classInstance->Weapon3);
	script->AddDragBoxUIButton("Weapon4", &classInstance->Weapon4);
	script->AddDragBoxUIButton("Weapon6", &classInstance->Weapon6);

	script->AddDragBoxMaterialComponent("SelectedWeapon", &classInstance->SelectedWeapon);

	script->AddDragBoxTextureResource("Material Weapon 1", &classInstance->Weapon1Texture);
	script->AddDragBoxTextureResource("Material Weapon 2", &classInstance->Weapon2Texture);
	script->AddDragBoxTextureResource("Material Weapon 3", &classInstance->Weapon3Texture);
	script->AddDragBoxTextureResource("Material Weapon 4", &classInstance->Weapon4Texture);
	script->AddDragBoxTextureResource("Material Weapon 5", &classInstance->Weapon5Texture);
	script->AddDragBoxTextureResource("Material Weapon 6", &classInstance->Weapon6Texture);

	script->AddDragBoxGameObject("Panel Upgrate W1", &classInstance->PanelUpgrate1);

	script->AddDragBoxGameObject("Panel Upgrate W2", &classInstance->PanelUpgrate2);
	script->AddDragBoxGameObject("Panel Unlock W2", &classInstance->PanelUnlock2);

	script->AddDragBoxGameObject("Panel Upgrate W3", &classInstance->PanelUpgrate3);
	script->AddDragBoxGameObject("Panel Unlock W3", &classInstance->PanelUnlock3);

	script->AddDragBoxGameObject("Panel Upgrate W4", &classInstance->PanelUpgrate4);
	script->AddDragBoxGameObject("Panel Unlock W4", &classInstance->PanelUnlock4);

	script->AddDragBoxGameObject("Panel Upgrate W5", &classInstance->PanelUpgrate5);
	script->AddDragBoxGameObject("Panel Unlock W5", &classInstance->PanelUnlock5);

	script->AddDragBoxGameObject("Panel Upgrate W6", &classInstance->PanelUpgrate6);
	script->AddDragBoxGameObject("Panel Unlock W6", &classInstance->PanelUnlock6);

	//create and add image api and change the image texture

	return classInstance;
}

void ArmoryWeaponSelect::Start()
{

}
void ArmoryWeaponSelect::Update()
{
	if (Weapon1.OnHovered())
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
		isUnlockWeapon2 = true;
	}

	if (Weapon3.OnPress())
	{
		//isUnlockWeapon3 = true;
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
	}
}

void ArmoryWeaponSelect::UnlockWeapon2()
{

}