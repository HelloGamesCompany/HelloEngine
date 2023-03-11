#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include <vector>

#include "API/API.h"

#include "Projectile.h"

class ProjectilePull : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_GameObject GetFirstActiveProjectile();
    void LauchProjectile(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_MeshRenderer projectileMesh);

    int pullSize;
    std::vector<API_GameObject> pull;

    // temp
    API_MeshRenderer mesh;

    API_GameObject projectileGO;
    API_GameObject playerGO;
};