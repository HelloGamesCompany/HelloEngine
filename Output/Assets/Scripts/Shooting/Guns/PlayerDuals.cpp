#include "PlayerDuals.h"
HELLO_ENGINE_API_C PlayerDuals* CreatePlayerDuals(ScriptToInspectorInterface* script)
{
    PlayerDuals* classInstance = new PlayerDuals();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Projectile Pull", &classInstance->projectilePull);
    script->AddDragFloat("Projectile Speed", &classInstance->projectileSpeed);
    script->AddDragFloat("Projectile Damage", &classInstance->projectileDamage);
    script->AddDragFloat("Projectile Resistance Damage", &classInstance->projectileResistanceDamage);
    script->AddDragFloat("Projectile Lifetime", &classInstance->projectileLifetime);
    script->AddDragBoxTransform("Projectile Spawn", &classInstance->shootingSpawn);
    script->AddDragBoxTransform("Second Projectile Spawn", &classInstance->secondShootingSpawn);
    script->AddDragBoxMeshRenderer("Projectile Mesh", &classInstance->projectileMesh);
    script->AddDragFloat("Projectiles per second", &classInstance->cadence);
    script->AddDragFloat("Burst Space", &classInstance->fullBurstDelay);
    return classInstance;
}

void PlayerDuals::Start()
{
    if (cadence != 0) fullShotCooldown = 1 / cadence;
    else fullShotCooldown = 0;
}

void PlayerDuals::Update()
{
    if (shotBuffer)
    {
        shotBufferCooldown -= Time::GetDeltaTime();
        if (shotBufferCooldown <= 0)
        {
            shotBuffer = false;
        }
    }

    // burst
    if (nextShot)
    {
        if (burstDelay <= 0)
        {
            nextShot = false;
            LauchProjectile(secondShootingSpawn);
        }
        else
        {
            burstDelay -= Time::GetDeltaTime();
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

void PlayerDuals::Shoot()
{
    if (canShoot)
    {
        LauchProjectile(shootingSpawn);
        canShoot = false;
        shotCooldown = fullShotCooldown;
        nextShot = true;
        burstDelay = fullBurstDelay;
    }
    else
    {
        shotBuffer = true;
        shotBufferCooldown = SHOT_BUFFER;
    }
}