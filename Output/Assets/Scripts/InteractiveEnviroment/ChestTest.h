#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "../Shooting/PlayerGunManager.h"

class ChestTest : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    //void OnCollisionStay(API::API_RigidBody other) override;

    int gunInsideChest;
    int gunInsideChestSlot;
};

