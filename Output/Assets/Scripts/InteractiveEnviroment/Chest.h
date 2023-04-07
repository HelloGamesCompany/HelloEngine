#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "../Shooting/PlayerGunManager.h"
#include "../Player/PlayerStats.h"
#include "../Player/PlayerMove.h"

class Chest : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(API::API_RigidBody other) override;

    API_AnimationPlayer chestAnimatorPlayer;

    float openChestTime;
    float maxOpenChestTime = 1.0f;
    bool opening;
    PlayerGunManager* playerGunManager;
    PlayerStats* playerStats;
    PlayerMove* playerMove;

    int itemIndex;
    // 0 -> Upgrade Blueprint
    // 1 -> Unlock SemiAutomatic
    // 2 -> Unlock Automatic
    // 3 -> Unlock Burst
    // 4 -> Unlock Shotgun
    // 5 -> Unlock Handgun
    // 6 -> Get Flamethrower
    // 7 -> Get Ricochet
};