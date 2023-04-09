#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "ArmoryWeaponSelect.h"

class ArmoryBluePrints : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_GameObject Weapon;

	API_Material bluePrint;

	API_MeshRenderer bluePrintMR;

	ArmoryWeaponSelect* WeaponUnlocked;

	bool textureChanged;
};

