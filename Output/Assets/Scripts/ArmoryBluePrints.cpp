#include "ArmoryBluePrints.h"
HELLO_ENGINE_API_C ArmoryBluePrints* CreateArmoryBluePrints(ScriptToInspectorInterface* script)
{
	ArmoryBluePrints* classInstance = new ArmoryBluePrints();

	script->AddDragBoxMaterialComponent("BluePrint Image Component", &classInstance->bluePrint);

	script->AddDragBoxGameObject("Related Weapon", &classInstance->Weapon);

	return classInstance;
}

void ArmoryBluePrints::Start()
{
	WeaponUnlocked = (ArmoryWeaponSelect*)Weapon.GetScript("ArmoryWeaponSelect");
	textureChanged = false;
}
void ArmoryBluePrints::Update()
{
	if (WeaponUnlocked->isUnlocked && textureChanged == false)
	{
		bluePrint.GetGameObject().GetMeshRenderer().SetActive(true);
		textureChanged = true;
	}
}