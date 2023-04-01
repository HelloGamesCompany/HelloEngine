#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

enum class PowerUp_Type
{
	SPEED_INCREASE,
	FIRERATE_INCREASE,
	SHIELD,
	MAX_AMMO,
	SLOW_TIME,
};


class HUD_Power_Up_Scrip : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void AddPowerUp(PowerUp_Type Powertype);

	API_GameObject Gameobjects_Pickables[5];
	uint Texture_Pick_up[5];
	API_Material Material_Pick_up[5];
	int Number_Picked = 0;
	//bool activeDash = true;

	uint Current_texture[5];

	float timer_power_up[5];
	float livetime_Power = 5.0f;
};

