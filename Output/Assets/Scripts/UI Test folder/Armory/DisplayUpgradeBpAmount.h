#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_UIText.h"

class PlayerStorage;

class DisplayUpgradeBpAmount : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_UIText bluePrintAmountText;

    API_GameObject playerStorageGO;
    PlayerStorage* playerStorage;
};

