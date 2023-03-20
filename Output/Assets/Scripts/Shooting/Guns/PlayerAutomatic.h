#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "../PlayerGun.h"

class PlayerAutomatic : public PlayerGun
{
public:
    void Start() override;
    void Update() override;

    void Shoot() override;
    void EnableGuns(bool enable) override;

    float fullShotCooldown;
    float shotCooldown;
    bool canShoot = true;
};

