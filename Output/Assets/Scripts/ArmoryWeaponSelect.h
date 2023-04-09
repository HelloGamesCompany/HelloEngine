#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_UIInput.h"

class ArmoryWeaponSelect : HelloBehavior
{
public:
	void Start() override;
	void Update() override;

	void UnlockWeapon2();

	API_UIButton CurrentWeapon;

	API_UIInput SelectWeaponList;

	API_GameObject NextWeapon;

	API_GameObject PrevtWeapon;

	API_UIInput CurrentPanelUpgrate;
	API_UIInput CurrentPanelUnlock;

	API_GameObject NextPanelUpgrate;
	API_GameObject NextPanelUnlock;

	API_GameObject PrevPanelUpgrate;
	API_GameObject PrevPanelUnlock;

	uint CurrentTextureWeapon;
	uint CurrentTextureWeaponLocked;

	bool isUnlocked = false;

	API_Material SelectedWeapon;

	int unlockPress = 0;

private:

	ArmoryWeaponSelect* nextW;
	ArmoryWeaponSelect* PrevW;
};

