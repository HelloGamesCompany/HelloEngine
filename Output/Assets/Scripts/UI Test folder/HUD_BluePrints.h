#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API_UIText.h"
#include "API/API.h"

class HUD_BluePrints : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void UpgradeAlert(int GetUpgrade);
	void New_WeaponAlert(int GetNewWeapon);
	void Special_WeaponAlert(int GetSpecialWeapon);

	API_Vector3 upgrade_movmentX;
	API_Vector3 new_weapon_movmentX;
	API_Vector3 special_weapon_movmentX;

	API_Material material_upgrade;
	API_Material material_Background_1;
	API_Material material_Background_2;
	API_Material material_Background_3;
	API_Material material_new_weapon;
	API_Material material_special_weapon;
	
	API_UIText text_upgrade;
	API_UIText text_new_weapon;
	API_UIText text_special_weapon;

	float timer_upgrade = 1.3f;
	float timer_new = 1.3f;
	float timer_special = 1.3f;

	uint upgrade_textures;
	uint new_weapon_textures[5];
	uint special_weapon_textures[2];

	API_Vector3 upgrade_position_save;
	API_Vector3 new_weapon_position_save;
	API_Vector3 special_weapon_position_save;
	API_Vector3 text_upgrade_position_save;
	API_Vector3 text_new_weapon_position_save;
	API_Vector3 text_special_weapon_position_save;
	API_Vector3 Background_upgrade_position_save;
	API_Vector3 Background_new_weapon_position_save;
	API_Vector3 Background_special_weapon_position_save;

};

