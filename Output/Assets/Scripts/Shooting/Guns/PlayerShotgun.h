#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "../PlayerGun.h"

class PlayerShotgun : public PlayerGun
{
public:
    void Start() override;
    void Update() override;

    void Shoot() override;
    void EnableGuns(bool enable) override;

    float fullShotCooldown;
    float shotCooldown;
    bool canShoot = true;
    int pellets = 0;
};

