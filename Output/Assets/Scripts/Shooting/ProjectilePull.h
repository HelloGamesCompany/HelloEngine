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
    API_GameObject playerGO;

    // temp
    API_MeshRenderer mesh;
};