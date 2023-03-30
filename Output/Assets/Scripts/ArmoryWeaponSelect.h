#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "ArmoryUpgratteButtons.h"

class ArmoryWeaponSelect : HelloBehavior
{
public:
	void Start() override;
	void Update() override;

	void UnlockWeapon2();

	API_UIButton CurrentWeapon;

	API_GameObject NextWeapon;

	API_GameObject PrevtWeapon;

	API_GameObject CurrentPanelUpgrate;
	API_GameObject CurrentPanelUnlock;

	API_GameObject NextPanelUpgrate;
	API_GameObject NextPanelUnlock;

	API_GameObject PrevPanelUpgrate;
	API_GameObject PrevPanelUnlock;

	uint CurrentTextureWeapon;

	bool isUnlocked = false;

	API_Material SelectedWeapon;


private:

	ArmoryWeaponSelect* nextW;
	ArmoryWeaponSelect* PrevW;
};

