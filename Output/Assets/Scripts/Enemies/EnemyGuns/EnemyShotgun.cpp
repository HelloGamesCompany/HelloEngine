#include "EnemyShotgun.h"
HELLO_ENGINE_API_C EnemyShotgun* CreateEnemyShotgun(ScriptToInspectorInterface* script)
{
    EnemyShotgun* classInstance = new EnemyShotgun();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Self Enemy GO", &classInstance->enemyGO);
    script->AddDragBoxGameObject("Projectile Pull", &classInstance->projectilePull);
    script->AddDragFloat("Projectile Speed", &classInstance->projectileSpeed);
    script->AddDragFloat("Projectile Damage", &classInstance->projectileDamage);
    script->AddDragFloat("Projectile Resistance Damage", &classInstance->projectileResistanceDamage);
    script->AddDragFloat("Projectile Lifetime", &classInstance->projectileLifetime);
    script->AddDragBoxTransform("Projectile Spawn", &classInstance->shootingSpawn);
    script->AddDragBoxMeshResource("Projectile Mesh", &classInstance->projectileMesh);
    script->AddDragBoxTextureResource("Projectile Material", &classInstance->projectileMaterial);
    script->AddDragFloat("Projectile ScaleX", &classInstance->projectileScale.x);
    script->AddDragFloat("Projectile ScaleY", &classInstance->projectileScale.y);
    script->AddDragFloat("Projectile ScaleZ", &classInstance->projectileScale.z);
    script->AddDragFloat("Projectiles per second", &classInstance->cadence);
    script->AddDragInt("Pellets per shot", &classInstance->pellets);
    script->AddDragFloat("Pellets Dispersion", &classInstance->pelletsDisersion);
    script->AddInputBox("Audio Event String", &classInstance->audioEventString);
    return classInstance;
}

void EnemyShotgun::Start()
{
    if (cadence != 0) fullShotCooldown = 1 / cadence;
    else fullShotCooldown = 0;
}

void EnemyShotgun::Update()
{
    if (canShoot) return;

    if (shotCooldown <= 0)
    {
        canShoot = true;
    }
    else
    {
        shotCooldown -= Time::GetDeltaTime();
    }
}

void EnemyShotgun::Shoot()
{
    if (canShoot)
    {
        for (size_t i = 0; i < pellets; i++)
        {
            LauchProjectile(shootingSpawn, pelletsDisersion);
        }
        PlayShotSound(audioEventString);
        canShoot = false;
        shotCooldown = fullShotCooldown;
    }
}