#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "../PlayerGun.h"

class PlayerHandgun : public PlayerGun
{
public:
    void Start() override;
    void Update() override;

    void Shoot() override;
    void EnableGuns(bool enable) override;
    void SetGunStatsPerLevel(int level) override;

    float fullShotCooldown;
    float shotCooldown;
    float fullShotCooldownWithPowerUp;
    bool canShoot = true;
    float shotBufferCooldown = SHOT_BUFFER;
    bool shotBuffer = false;
};