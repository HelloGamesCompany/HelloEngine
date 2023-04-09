#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_UIInput.h"
#include "ArmoryWeaponSelect.h"

class ArmoryUnlock : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_UIButton Unlock;
    API_UIInput PanelUpgrate;
    API_UIInput PanelUnlock;

    API_GameObject currentWeapon;
    API_UIInput SelectWeaponList;

    ArmoryWeaponSelect* weaponInstance;

    bool haveBlueprint = false;
    int gunIndex;
};

