#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "../Player/PlayerStats.h"

class HpBar : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    float FirstPosX;
    float FirstPosY;
    float FirstPosZ;

    API_GameObject playerStatsGO;
    PlayerStats* playerStats;
};

