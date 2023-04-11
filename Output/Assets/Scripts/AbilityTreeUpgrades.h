#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "Player/PlayerStorage.h"

class AbilityTreeUpgrades : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_UIButton buttonHovered;
    API_UIInput mainPanel;
    API_UIInput currentPanel;
    int upgradeIndex;
    int treeIndex;

    API_GameObject playerStorageGO;
    PlayerStorage* playerStorage;

    float manteinTime = 0.0f;

    int skillLevel;
};

