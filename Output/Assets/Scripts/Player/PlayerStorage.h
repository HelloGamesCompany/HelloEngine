#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStorage : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void SaveData();

    int unlockGunBlueprint;
    int upgradeBlueprintAmount;

    // casettes
    bool casette1Picked;
    bool casette2Picked;
    bool casette3Picked;
};

