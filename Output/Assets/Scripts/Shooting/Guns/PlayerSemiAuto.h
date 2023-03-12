#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "../PlayerGun.h"

class PlayerSemiAuto : public PlayerGun
{
public:
    void Start() override;
    void Update() override;

    void Shoot() override;

    float fullShotCooldown;
    float shotCooldown;
    bool canShoot = true;
    float shotBufferCooldown = SHOT_BUFFER;
    bool shotBuffer = false;;
};