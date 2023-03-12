#pragma once

#define SHOT_BUFFER 0.5f

#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "ProjectilePull.h"
#include "../PlayerGamepadMovement.h"

class PlayerGun : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    virtual void Shoot();

protected:
    void LauchProjectile(API_Transform projectileSpawn);

public:
    API_GameObject projectilePull;

    float projectileSpeed;
    float projectileDamage;
    float projectileResistanceDamage;
    float projectileLifetime;
    API_Transform shootingSpawn;
    API_MeshRenderer projectileMesh;

    float cadence;
};

