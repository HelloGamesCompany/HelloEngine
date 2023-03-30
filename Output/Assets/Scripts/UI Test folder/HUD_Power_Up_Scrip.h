#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class HUD_Power_Up_Scrip : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void GetPowerUp(uint texture);

	//Pick up 1  
	
	//API_Material Material_Pick_up_1;
	//API_GameObject Gameobject_Pick_up_1;

	//Pick up 2  
	//uint Texture_Pick_up_2;
	//API_Material Material_Pick_up_2;
	//API_GameObject Gameobject_Pick_up_2;

	//Pick up 3   
	//uint Texture_Pick_up_3;
	//API_Material Material_Pick_up_3;
	//API_GameObject Gameobject_Pick_up_3;

	//Pick up 4
	//uint Texture_Pick_up_4;
	//API_Material Material_Pick_up_4;
	//API_GameObject Gameobject_Pick_up_4;

	//Pick up 5
	//uint Texture_Pick_up_5;
	//API_Material Material_Pick_up_5;
	//API_GameObject Gameobject_Pick_up_5;

	API_GameObject Gameobjects_Pickables[5];
	uint Texture_Pick_up[5];
	API_Material Material_Pick_up[5];
	int Number_Picked = 0;
	//bool activeDash = true;

};

