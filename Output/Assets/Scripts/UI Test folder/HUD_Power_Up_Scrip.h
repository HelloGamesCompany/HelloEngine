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

	//Pick up 1  
	uint Texture_Pick_up_1;
	API_Material Material_Pick_up_1;
	API_GameObject Gameobject_Pick_up_1;

	//Pick up 2  
	uint Texture_Pick_up_2;
	API_Material Material_Pick_up_2;
	API_GameObject Gameobject_Pick_up_2;

	//Pick up 3   
	uint Texture_Pick_up_3;
	API_Material Material_Pick_up_3;
	API_GameObject Gameobject_Pick_up_3;

	//Pick up 4
	uint Texture_Pick_up_4;
	API_Material Material_Pick_up_4;
	API_GameObject Gameobject_Pick_up_4;

	//Pick up 5
	uint Texture_Pick_up_5;
	API_Material Material_Pick_up_5;
	API_GameObject Gameobject_Pick_up_5;

	API_GameObject Gameobjects_Pickables[4];

	int Number_Picked;
	//bool activeDash = true;

};

