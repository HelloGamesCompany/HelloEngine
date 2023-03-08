#include "PlayerSemiAuto.h"
HELLO_ENGINE_API_C PlayerSemiAuto* CreatePlayerSemiAuto(ScriptToInspectorInterface* script)
{
    PlayerSemiAuto* classInstance = new PlayerSemiAuto();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Projectile Pull", &classInstance->projectilePull);
    script->AddDragFloat("Projectile Speed", &classInstance->projectileSpeed);
    script->AddDragFloat("Projectile Damage", &classInstance->projectileDamage);
    script->AddDragFloat("Projectile Resistance Damage", &classInstance->projectileResistanceDamage);
    script->AddDragFloat("Projectile Lifetime", &classInstance->projectileLifetime);
    script->AddDragBoxTransform("Projectile Spawn", &classInstance->shootingSpawn);
    script->AddDragBoxMeshRenderer("Projectile Mesh", &classInstance->projectileMesh);
    script->AddDragFloat("Projectiles per second", &classInstance->cadence);
    script->AddDragBoxGameObject("Player", &classInstance->playerGO);
    return classInstance;
}

void PlayerSemiAuto::Start()
{
    if (cadence != 0) fullShotCooldown = 1 / cadence;
    else fullShotCooldown = 0;
}

void PlayerSemiAuto::Update()
{
    if (shotBuffer)
    {
        shotBufferCooldown -= Time::GetDeltaTime();
        if (shotBufferCooldown <= 0)
        {
            shotBuffer = false;
        }
    }

    if (canShoot)
    {
        if (shotBuffer)
        {
            Shoot();
        }
        return;
    }

    if (shotCooldown <= 0)
    {
        canShoot = true;
    }
    else
    {
        shotCooldown -= Time::GetDeltaTime();
    }
}

void PlayerSemiAuto::Shoot()
{
    if (canShoot)
    {
        LauchProjectile();
        canShoot = false;
        shotCooldown = fullShotCooldown;
    }
    else
    {
        shotBuffer = true;
        shotBufferCooldown = 0.5f;
    }
}