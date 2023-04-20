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

	void UpgradeAlert();
	void New_WeaponAlert();
	void Special_WeaponAlert();

	API_Vector3 upgrade_movmentX;
	API_Vector3 new_weapon_movmentX;
	API_Vector3 special_weapon_movmentX;


	API_Material material_upgrade;
	API_Material material_new_weapon;
	API_Material material_special_weapon;
	
	API_UIText text_upgrade;
	API_UIText text_new_weapon;
	API_UIText text_special_weapon;

	float timer_upgrade = 1.3f;
	float timer_new = 1.0f;
	float timer_special = 1.0f;

};

