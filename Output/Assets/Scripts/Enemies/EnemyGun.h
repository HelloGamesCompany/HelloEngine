#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "EnemyProjectilePull.h"
#include "EnemyProjectile.h"

class EnemyGun : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    virtual void Shoot();

protected:
    void LauchProjectile(API_Transform projectileSpawn, float randomDirectionRange = 0.0f);
    void PlayShotSound(API_String eventString);

public:
    API_GameObject enemyGO;
    API_GameObject projectilePull;

    float projectileSpeed;
    float projectileDamage;
    float projectileResistanceDamage;
    float projectileLifetime;
    API_Transform shootingSpawn;
    uint projectileMesh;
    uint projectileMaterial;
    API_Vector3 projectileScale;

    float cadence;

    API_String audioEventString;
};

