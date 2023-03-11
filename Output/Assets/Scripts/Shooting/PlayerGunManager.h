#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "PlayerGun.h"
#include "PlayerGunType.h"

class PlayerGunManager : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_GameObject guns;
    PlayerGun* equipedGun;

    bool canShoot = true;
};

