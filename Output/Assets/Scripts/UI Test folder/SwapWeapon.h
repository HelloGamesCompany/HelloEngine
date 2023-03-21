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

	uint Texture_Weapon;
	API_Material Material_Weapon;
};

