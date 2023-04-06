#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "SwapWeapon.h"
#include "Macro.h"
#include "API/API_UIText.h"
#include "API/API.h"

class UI_Municion : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;


	void GetMunitionInfinit(bool Weaponactive);
	void GetMunitionNormal(normalWeapon_Type WeaponType,int munition_Normal, bool Weaponactive);
	void GetMunitionSpecial(specialWeapon_Type WeaponType,int munition_Special, bool Weaponactive);

	API_GameObject text_munition_InfinitWeapon_On;
	API_GameObject text_munition_InfinitWeapon_Off;
	API_UIText text_munition_Normal_On;
	API_UIText text_munition_Normal_Off;
	API_UIText text_munition_Special_On;
	API_UIText text_munition_Special_Off;
};

