#include "HUD_BluePrints.h"
HELLO_ENGINE_API_C HUD_BluePrints* CreateHUD_BluePrints(ScriptToInspectorInterface* script)
{
	HUD_BluePrints* classInstance = new HUD_BluePrints();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxMaterialComponent("Material_Upgrade", &classInstance->material_upgrade);
	script->AddDragBoxMaterialComponent("Material_New_Weapon", &classInstance->material_new_weapon);
	script->AddDragBoxMaterialComponent("Material_Special_Weapon", &classInstance->material_special_weapon);

	//
	script->AddDragBoxUIText("Text_Upgrade", &classInstance->text_upgrade);
	script->AddDragBoxUIText("Text_Upgrade", &classInstance->text_new_weapon);
	script->AddDragBoxUIText("Text_Upgrade", &classInstance->text_special_weapon);

	return classInstance;
}

void HUD_BluePrints::Start()
{
	upgrade_movmentX = {-0.0015,0,0};
	new_weapon_movmentX = { -0.0015,0,0 };
	special_weapon_movmentX = { -0.0015,0,0 };
}
void HUD_BluePrints::Update()
{

	//UpgradeAlert();
}

void HUD_BluePrints::UpgradeAlert()
{
	timer_upgrade -= Time::GetDeltaTime();

	if (timer_upgrade >= 0.0f)
	{

		material_upgrade.GetGameObject().GetTransform().Translate(upgrade_movmentX);
		text_upgrade.GetGameObject().GetTransform().Translate(upgrade_movmentX);
	}
	else if (timer_upgrade <= -3.0f)
	{
		material_upgrade.GetGameObject().SetActive(false);
		text_upgrade.GetGameObject().SetActive(false);
	}
}

void HUD_BluePrints::New_WeaponAlert()
{
	timer_new -= Time::GetDeltaTime();

	if (timer_new >= 0.0f)
	{

		material_new_weapon.GetGameObject().GetTransform().Translate(new_weapon_movmentX);
		text_upgrade.GetGameObject().GetTransform().Translate(new_weapon_movmentX);
	}
	else if (timer_new <= -3.0f)
	{
		material_new_weapon.GetGameObject().SetActive(false);
		text_upgrade.GetGameObject().SetActive(false);
	}
}

void HUD_BluePrints::Special_WeaponAlert()
{
	timer_special -= Time::GetDeltaTime();

	if (timer_special >= 0.0f)
	{

		material_special_weapon.GetGameObject().GetTransform().Translate(special_weapon_movmentX);
		text_special_weapon.GetGameObject().GetTransform().Translate(special_weapon_movmentX);
	}
	else if (timer_special <= -3.0f)
	{
		material_special_weapon.GetGameObject().SetActive(false);
		text_special_weapon.GetGameObject().SetActive(false);
	}
}
