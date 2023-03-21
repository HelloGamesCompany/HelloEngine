#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class SwapWeapon : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void SwapWeapon1();
	void SwapWeapon2();
	void SwapWeapon3();
	void Dash();

	//Weapon 1  
	uint Texture_Weapon_1_on;
	uint Texture_Weapon_1_off;
	API_Material Material_Weapon_1;
	
	//Weapon 2 
	uint Texture_Weapon_2_on;
	uint Texture_Weapon_2_off;
	API_Material Material_Weapon_2;
	
	//Weapon 3  
	uint Texture_Weapon_3_on;
	uint Texture_Weapon_3_off;
	API_Material Material_Weapon_3;

	API_GameObject Active_Bar_1;
	API_GameObject Active_Bar_2;
	API_GameObject Active_Bar_3;

	//Dash
	uint Texture_Dash_on;
	uint Texture_Dash_off;
	API_Material Material_Dash;
	bool activeDash = true;
};

