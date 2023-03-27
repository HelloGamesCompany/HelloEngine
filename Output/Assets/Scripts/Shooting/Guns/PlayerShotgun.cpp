#include "PlayerShotgun.h"
HELLO_ENGINE_API_C PlayerShotgun* CreatePlayerShotgun(ScriptToInspectorInterface* script)
{
    PlayerShotgun* classInstance = new PlayerShotgun();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
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
    script->AddDragFloat("Extra % firerate", &classInstance->upgradeFireratePercentage);
    script->AddDragInt("Pellets per shot", &classInstance->pellets);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->player);
    script->AddDragInt("Ammo Type", &classInstance->ammoType);
    script->AddInputBox("Audio Event String", &classInstance->audioEventString);
    return classInstance;
}

void PlayerShotgun::Start()
{
    playerStats = (PlayerStats*)player.GetScript("PlayerStats");

    if (cadence != 0) fullShotCooldown = 1 / cadence;
    else fullShotCooldown = 0;

    if (playerStats->armoryTreeLvl > 1)
    {
        fullShotCooldown = fullShotCooldown + fullShotCooldown * upgradeFireratePercentage / 100.0f;
    }

    if (cadence != 0) fullShotCooldownWithPowerUp = 1 / (cadence * 1.5f); // 50% increase
    else fullShotCooldownWithPowerUp = 0;

    if (playerStats->armoryTreeLvl > 1)
    {
        fullShotCooldownWithPowerUp = fullShotCooldownWithPowerUp + fullShotCooldownWithPowerUp * upgradeFireratePercentage / 100.0f;
    }
}

void PlayerShotgun::Update()
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

void PlayerShotgun::Shoot()
{
    if (canShoot)
    {
        for (size_t i = 0; i < pellets; i++)
        {
            LauchProjectile(shootingSpawn, PROJECTILE_ACTION::NONE, true);
        }
        PlayShotSound(audioEventString);
        canShoot = false;
        if (playerStats->fireratePowerUp) shotCooldown = fullShotCooldownWithPowerUp;
        else shotCooldown = fullShotCooldown;
        playerStats->UseAmmo(ammoType);
    }
    else
    {
        shotBuffer = true;
        shotBufferCooldown = SHOT_BUFFER;
    }
}

void PlayerShotgun::EnableGuns(bool enable)
{
    gameObject.SetActive(enable);
    shotBuffer = false;
}