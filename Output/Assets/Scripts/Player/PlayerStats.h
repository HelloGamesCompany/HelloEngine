#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStats : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    float maxHp;
    float currentHp;
    void TakeDamage(float amount);

    int GetAmmonByType(int type);
    void GetAmmo(int type, int amount);
    void UseAmmo(int type, int amount = 1);

    int laserAmmo; // index 1
    int maxLaserAmmo;
    int fireAmmo; // index 2
    int maxFireAmmo;
    int ricochetAmmo; // index 3
    int maxRicochetAmmo;
};

