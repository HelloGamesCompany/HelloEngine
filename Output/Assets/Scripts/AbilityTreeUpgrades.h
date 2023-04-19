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

    void UpgradeSkill();
    int upgradeCost;

    API_UIButton Upgrate1;
    int skillPoints1;
    API_UIButton Upgrate2;
    int skillPoints2;
    API_UIButton Upgrate3;
    int skillPoints3;
    API_UIButton Upgrate4;
    int skillPoints4;
    API_UIButton Upgrate5;
    int skillPoints5;
    int upgradeNum;

    API_UIInput mainPanel;
    API_UIInput currentPanel;

    API_GameObject description1;
    API_GameObject description2;
    API_GameObject description3;
    API_GameObject description4;
    API_GameObject description5;


    int skillLevel;
    int treeIndex;

    API_GameObject playerStorageGO;
    PlayerStorage* playerStorage;

    float manteinTime = 0.0f;
    int upgradingLevel;

};

