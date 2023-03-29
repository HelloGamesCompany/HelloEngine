#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Pick_up_Scrip : HelloBehavior
{
public:
	void Start() override;
	void Update() override;


	//Pick up 1  
	uint Texture_Pick_up_1_on;
	uint Texture_Pick_up_1_off;
	API_Material Material_Pick_up_1;
	API_GameObject Gameobject_Pick_up_1;
	
	//Pick up 2  
	uint Texture_Pick_up_2_on;
	uint Texture_Pick_up_2_off;
	API_Material Material_Pick_up_2;
	API_GameObject Gameobject_Pick_up_2;

	//Pick up 3   
	uint Texture_Pick_up_3_on;
	uint Texture_Pick_up_3_off;
	API_Material Material_Pick_up_3;
	API_GameObject Gameobject_Pick_up_3;
	
	//Pick up 4
	uint Texture_Pick_up_4_on;
	uint Texture_Pick_up_4_off;
	API_Material Material_Pick_up_4;
	API_GameObject Gameobject_Pick_up_4;
	
	//Pick up 5
	uint Texture_Pick_up_5_on;
	uint Texture_Pick_up_5_off;
	API_Material Material_Pick_up_5;
	API_GameObject Gameobject_Pick_up_5;
	 
	//bool activeDash = true;
};

