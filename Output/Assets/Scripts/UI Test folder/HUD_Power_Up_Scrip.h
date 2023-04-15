#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "../Player/PlayerStats.h"

enum class PowerUp_Type
{
    SPEED_INCREASE,
    FIRERATE_INCREASE,
    SHIELD,
    SLOW_TIME,
};


class HUD_Power_Up_Scrip : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void RealocatePowerUps(int removedIndex);
    void AddPowerUp(PowerUp_Type Powertype, float duration);
    void RemovePowerUp(PowerUp_Type Powertype);

    API_GameObject Gameobjects_Pickables[4];
    uint Texture_Pick_up[4];
    API_Material Material_Pick_up[4];
    int Number_Picked = 0;
    uint Current_texture[4];

    float timer_power_up[4];
    float max_timer_power_up[4];

    API_GameObject playerStatsGO;
    PlayerStats* playerStats;
};