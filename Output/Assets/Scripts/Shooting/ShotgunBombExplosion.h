#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ShotgunBombExplosion : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionStay(API::API_RigidBody other);

    bool triggerActive;
    API_GameObject shotgunBomb; // parent
    bool destroy = false;

    float damage = 1.0f;
    float resistanceDamage = 1.0f;
};

