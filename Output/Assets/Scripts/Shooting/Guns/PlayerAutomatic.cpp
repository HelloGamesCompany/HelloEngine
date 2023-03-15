#include "PlayerAutomatic.h"
HELLO_ENGINE_API_C PlayerAutomatic* CreatePlayerAutomatic(ScriptToInspectorInterface* script)
{
    PlayerAutomatic* classInstance = new PlayerAutomatic();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Projectile Pull", &classInstance->projectilePull);
    script->AddDragFloat("Projectile Speed", &classInstance->projectileSpeed);
    script->AddDragFloat("Projectile Damage", &classInstance->projectileDamage);
    script->AddDragFloat("Projectile Resistance Damage", &classInstance->projectileResistanceDamage);
    script->AddDragFloat("Projectile Lifetime", &classInstance->projectileLifetime);
    script->AddDragBoxTransform("Projectile Spawn", &classInstance->shootingSpawn);
    script->AddDragBoxMeshRenderer("Projectile Mesh", &classInstance->projectileMesh);
    script->AddDragFloat("Projectile ScaleX", &classInstance->projectileScale.x);
    script->AddDragFloat("Projectile ScaleY", &classInstance->projectileScale.y);
    script->AddDragFloat("Projectile ScaleZ", &classInstance->projectileScale.z);
    script->AddDragFloat("Projectiles per second", &classInstance->cadence);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->player);
    script->AddDragInt("Ammo Type", &classInstance->ammoType);
    return classInstance;
}

void PlayerAutomatic::Start()
{
    playerStats = (PlayerStats*)player.GetScript("PlayerStats");

    if (cadence != 0) fullShotCooldown = 1 / cadence;
    else fullShotCooldown = 0;
}

void PlayerAutomatic::Update()
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

void PlayerAutomatic::Shoot()
{
    if (canShoot)
    {
        LauchProjectile(shootingSpawn);
        canShoot = false;
        shotCooldown = fullShotCooldown;
        playerStats->UseAmmo(ammoType);
    }
}

void PlayerAutomatic::EnableGuns(bool enable)
{
    gameObject.SetActive(enable);
}