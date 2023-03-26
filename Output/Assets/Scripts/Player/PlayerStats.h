#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class HpBar;

class PlayerStats : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    float currentMaxHp;
    float maxHp;
    float upgradedMaxHp;
    float currentHp;
    void TakeDamage(float amount);
    float inmunityTime = 2.0f;
    bool secondLife;
    float lastHitTime;

    int GetAmmonByType(int type);
    void GetAmmo(int type, int amount);
    void UseAmmo(int type, int amount = 1);

    int laserAmmo; // index 1
    int maxLaserAmmo;
    int fireAmmo; // index 2
    int maxFireAmmo;
    int ricochetAmmo; // index 3
    int maxRicochetAmmo;

    bool detected;//if the player is being detected somehow

    API_GameObject hpGameObject;
    HpBar* healthBar;

    // skills tree
    void UpgradeTreeLvl(int tree);

    int movementTreeLvl; // tree 0
    int armoryTreeLvl; // tree 1
    int healthTreeLvl; // tree 2
    int specialTreeLvl; // tree 3
};

