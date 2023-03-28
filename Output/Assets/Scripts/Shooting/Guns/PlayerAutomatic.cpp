#include "PlayerAutomatic.h"
HELLO_ENGINE_API_C PlayerAutomatic* CreatePlayerAutomatic(ScriptToInspectorInterface* script)
{
    PlayerAutomatic* classInstance = new PlayerAutomatic();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Projectile Pull", &classInstance->projectilePull);
    //script->AddCheckBox("Testing", &classInstance->testing);
    //if (&classInstance->testing)
    {
        script->AddDragFloat("Projectile Speed", &classInstance->projectileSpeed);
        script->AddDragFloat("Projectile Damage", &classInstance->projectileDamage);
        script->AddDragFloat("Projectile Resistance Damage", &classInstance->projectileResistanceDamage);
        script->AddDragFloat("Projectile Lifetime", &classInstance->projectileLifetime);
    }
    
    script->AddDragBoxTransform("Projectile Spawn", &classInstance->shootingSpawn);
    script->AddDragBoxMeshResource("Projectile Mesh", &classInstance->projectileMesh);
    script->AddDragBoxTextureResource("Projectile Material", &classInstance->projectileMaterial);
    script->AddDragFloat("Projectile ScaleX", &classInstance->projectileScale.x);
    script->AddDragFloat("Projectile ScaleY", &classInstance->projectileScale.y);
    script->AddDragFloat("Projectile ScaleZ", &classInstance->projectileScale.z);
    script->AddDragFloat("Projectiles per second", &classInstance->cadence);
    script->AddDragFloat("Extra % firerate", &classInstance->upgradeFireratePercentage);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->player);
    script->AddDragInt("Ammo Type", &classInstance->ammoType);
    script->AddInputBox("Audio Event String", &classInstance->audioEventString);
    return classInstance;
}

void PlayerAutomatic::Start()
{
    playerStats = (PlayerStats*)player.GetScript("PlayerStats");

    if (cadence == 0)
    {
        fullShotCooldown = 0;
        fullShotCooldownWithPowerUp = 0;
    }
    else
    {
        fullShotCooldown = 1 / cadence;
        fullShotCooldownWithPowerUp = 1 / (cadence * 1.5f); // 50% increase

        if (playerStats->armoryTreeLvl > 1)
        {
            fullShotCooldown = 1 / (cadence + cadence * upgradeFireratePercentage / 100.0f);
            fullShotCooldownWithPowerUp = 1 / ((cadence + cadence * upgradeFireratePercentage / 100.0f) * 1.5f); // 50% increase
        }
    }
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
        PlayShotSound(audioEventString);
        canShoot = false;
        if (playerStats->fireratePowerUp) shotCooldown = fullShotCooldownWithPowerUp;
        else shotCooldown = fullShotCooldown;
        playerStats->UseAmmo(ammoType);
    }
}

void PlayerAutomatic::EnableGuns(bool enable)
{
    gameObject.SetActive(enable);
}

void PlayerAutomatic::SetGunStatsPerLevel(int level)
{
    switch (level)
    {
    case 0:
        projectileSpeed = 40.0f;
        projectileDamage = 25.0f;
        projectileResistanceDamage = 0.0f;
        projectileLifetime = 1.0f;
        cadence = 5.0f;
        break;
    case 1:
        projectileSpeed = 40.0f;
        projectileDamage = 25.0f;
        projectileResistanceDamage = 0.0f;
        projectileLifetime = 1.0f;
        cadence = 5.0f;
        break;
    case 2:
        projectileSpeed = 40.0f;
        projectileDamage = 25.0f;
        projectileResistanceDamage = 0.0f;
        projectileLifetime = 1.0f;
        cadence = 5.0f;
        break;
    case 3:
        projectileSpeed = 40.0f;
        projectileDamage = 25.0f;
        projectileResistanceDamage = 0.0f;
        projectileLifetime = 1.0f;
        cadence = 5.0f;
        break;
    default:
        Console::Log("Automatic gun level can't be different from 0, 1, 2 or 3.");
        break;
    }
}