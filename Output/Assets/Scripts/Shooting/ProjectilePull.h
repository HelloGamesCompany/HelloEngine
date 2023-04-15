#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include <vector>

#include "API/API.h"

#include "Projectile.h"
#include "ShotgunBomb.h"

#define ELECTRICITY_DELAY 0.5f
#define ELECTRICITY_CHAIN_COUNT 3

class PlayerStats;

class ProjectilePull : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_GameObject GetFirstInactiveProjectile();
    API_GameObject GetFirstInactiveShotgunBomb();
    API_GameObject GetFirstInactiveElectricityChain();
    void LauchProjectileNORMAL(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction);
    void LauchProjectileSEMI(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction);
    void LauchProjectileSECONDARY_SEMI(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction, float rotateY);
    void LauchProjectileAUTO(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction);
    void LauchProjectileBURST(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction);
    void LauchProjectileSHOTGUN(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction, float randomDirectionRange);
    void LauchProjectileSHOTGUN_BOMB(float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction);
    void LauchProjectileHANDGUN(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction);
    void LauchProjectileELECTRICITY_CHAIN(float delay, float damage, float resistanceDamage, API_GameObject attachedToGO);

    int pullSize;
    std::vector<API_GameObject> pull;
    int shotgunBombPullSize;
    std::vector<API_GameObject> shotgunBombPull;
    int electricityChainPullSize;
    std::vector<API_GameObject> electricityChainPull;
    API_GameObject playerGO;
    PlayerStats* playerStats;
    uint projectilePrefab;
    uint shotgunBombPrefab;
    uint electricityChainPrefab;

    float autoForce = 0.0f;
};