#include "ArmoryWeaponSelect.h"
HELLO_ENGINE_API_C ArmoryWeaponSelect* CreateArmoryWeaponSelect(ScriptToInspectorInterface* script)
{
	ArmoryWeaponSelect* classInstance = new ArmoryWeaponSelect();

	script->AddDragBoxUIButton("Weapon1", &classInstance->Weapon1);
	script->AddDragBoxUIButton("Weapon2", &classInstance->Weapon2);
	script->AddDragBoxUIButton("Weapon3", &classInstance->Weapon3);
	script->AddDragBoxUIButton("Weapon4", &classInstance->Weapon4);
	script->AddDragBoxUIButton("Weapon5", &classInstance->Weapon5);
	script->AddDragBoxUIButton("Weapon6", &classInstance->Weapon6);
	script->AddDragBoxUIButton("Weapon7", &classInstance->Weapon7);
	script->AddDragBoxUIButton("Weapon8", &classInstance->Weapon8);
	//create and add image api and change the image texture

	return classInstance;
}

void ArmoryWeaponSelect::Start()
{

}
void ArmoryWeaponSelect::Update()
{
	if (Weapon1.OnPress())
	{
		
	}

	if (Weapon2.OnPress())
	{

	}

	if (Weapon3.OnPress())
	{

	}

	if (Weapon4.OnPress())
	{

	}

	if (Weapon5.OnPress())
	{

	}

	if (Weapon6.OnPress())
	{

	}

	if (Weapon7.OnPress())
	{

	}

	if (Weapon8.OnPress())
	{

	}
}