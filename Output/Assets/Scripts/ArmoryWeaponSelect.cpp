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
		Console::Log("1");
	}

	if (Weapon2.OnHovered())
	{
		SelectedWeapon.ChangeAlbedoTexture(Weapon2Texture);
		Console::Log("2");
	}

	if (Weapon3.OnHovered())
	{
		SelectedWeapon.ChangeAlbedoTexture(Weapon3Texture);
		Console::Log("3");
	}

	if (Weapon4.OnHovered())
	{
		SelectedWeapon.ChangeAlbedoTexture(Weapon4Texture);
		Console::Log("4");
	}

	if (Weapon5.OnHovered())
	{
		SelectedWeapon.ChangeAlbedoTexture(Weapon5Texture);
		Console::Log("5");
	}

	if (Weapon6.OnHovered())
	{
		SelectedWeapon.ChangeAlbedoTexture(Weapon6Texture);
		Console::Log("6");
	}
}