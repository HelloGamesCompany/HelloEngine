#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ArmoryWeaponSelect : HelloBehavior
{
public:
	void Start() override;
	void Update() override;

	void UnlockWeapon2();

	API_UIButton Weapon1;
	API_UIButton Weapon2;
	API_UIButton Weapon3;
	API_UIButton Weapon4;
	API_UIButton Weapon5;
	API_UIButton Weapon6;

	API_Material SelectedWeapon;
	uint Weapon1Texture;
	uint Weapon2Texture;
	uint Weapon3Texture;
	uint Weapon4Texture;
	uint Weapon5Texture;
	uint Weapon6Texture;

	API_GameObject PanelUpgrate1;

	API_GameObject PanelUpgrate2;
	API_GameObject PanelUnlock2;

	API_GameObject PanelUpgrate3;
	API_GameObject PanelUnlock3;

	API_GameObject PanelUpgrate4;
	API_GameObject PanelUnlock4;

	API_GameObject PanelUpgrate5;
	API_GameObject PanelUnlock5;

	API_GameObject PanelUpgrate6;
	API_GameObject PanelUnlock6;

	API_UIButton WeaponSelected;

	bool isUnlockWeapon2 = false;
	bool isUnlockWeapon3 = false;
	bool isUnlockWeapon4 = false;
	bool isUnlockWeapon5 = false;
	bool isUnlockWeapon6 = false;

private:


};

