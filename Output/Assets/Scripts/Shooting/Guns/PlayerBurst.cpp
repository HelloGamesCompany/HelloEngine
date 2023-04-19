#include "PlayerBurst.h"
HELLO_ENGINE_API_C PlayerBurst* CreatePlayerBurst(ScriptToInspectorInterface* script)
{
    PlayerBurst* classInstance = new PlayerBurst();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Projectile Pull", &classInstance->projectilePull);
    script->AddDragFloat("Projectile Speed", &classInstance->projectileSpeed);
    script->AddDragFloat("Projectile Damage", &classInstance->projectileDamage);
    script->AddDragFloat("Projectile Resistance Damage", &classInstance->projectileResistanceDamage);
    script->AddDragFloat("Projectile Lifetime", &classInstance->projectileLifetime);
    script->AddDragBoxTransform("Projectile Spawn", &classInstance->shootingSpawn);
    script->AddDragFloat("Projectile ScaleX", &classInstance->projectileScale.x);
    script->AddDragFloat("Projectile ScaleY", &classInstance->projectileScale.y);
    script->AddDragFloat("Projectile ScaleZ", &classInstance->projectileScale.z);
    script->AddDragFloat("Projectiles per second", &classInstance->cadence);
    script->AddDragFloat("Extra % firerate", &classInstance->upgradeFireratePercentage);
    script->AddDragFloat("Burst Space", &classInstance->fullBurstDelay);
    script->AddDragInt("Projectiles per burst", &classInstance->burstLenght);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->player);
    script->AddDragInt("Ammo Type", &classInstance->ammoType);
    script->AddInputBox("Audio Event String", &classInstance->audioEventString);
    return classInstance;
}

void PlayerBurst::Start()
{
    playerStats = (PlayerStats*)player.GetScript("PlayerStats");

    SetGunStatsPerLevel(API_QuickSave::GetInt("burst_level")); // read from save file

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

    shotCount = burstLenght;
}

void PlayerBurst::Update()
{
    float dt;
    if (playerStats->slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    if (shotBuffer)
    {
        shotBufferCooldown -= dt;
        if (shotBufferCooldown <= 0)
        {
            shotBuffer = false;
        }
    }

    // burst
    if (shotCount < burstLenght)
    {
        if (burstDelay <= 0)
        {
            shotCount++;
            burstDelay = fullBurstDelay;
            LauchProjectile(shootingSpawn, PROJECTILE_TYPE::BURST);
            PlayShotSound(audioEventString);
            playerStats->UseAmmo(ammoType);
        }
        else
        {
            burstDelay -= dt;
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
        shotCooldown -= dt;
    }
}

void PlayerBurst::Shoot()
{
    if (canShoot)
    {
        LauchProjectile(shootingSpawn, PROJECTILE_TYPE::BURST);
        PlayShotSound(audioEventString);
        canShoot = false;
        if (playerStats->fireratePowerUp) shotCooldown = fullShotCooldownWithPowerUp;
        else shotCooldown = fullShotCooldown;
        shotCount = 1;
        burstDelay = fullBurstDelay;
        playerStats->UseAmmo(ammoType);
    }
    else
    {
        shotBuffer = true;
        shotBufferCooldown = SHOT_BUFFER;
    }
}

void PlayerBurst::EnableGuns(bool enable)
{
    gameObject.SetActive(enable);
    shotBuffer = false;
    shotCount = burstLenght;
}

void PlayerBurst::SetGunStatsPerLevel(int level)
{
    switch (level)
    {
    case 0:
        projectileSpeed = 35.0f;
        projectileDamage = 20.0f;
        projectileResistanceDamage = 20.0f;
        projectileLifetime = 1.0f;
        cadence = 1.0f;
        burstLenght = 3;
        fullBurstDelay = 0.1f;
        break;
    case 1:
        projectileSpeed = 35.0f;
        projectileDamage = 20.0f;
        projectileResistanceDamage = 0.0f;
        projectileLifetime = 1.0f;
        cadence = 1.0f;
        burstLenght = 4;
        fullBurstDelay = 0.1f;
        break;
    case 2:
        projectileSpeed = 45.0f;
        projectileDamage = 20.0f;
        projectileResistanceDamage = 0.0f;
        projectileLifetime = 1.0f;
        cadence = 1.0f;
        burstLenght = 4;
        fullBurstDelay = 0.1f;
        break;
    case 3:
        projectileSpeed = 45.0f;
        projectileDamage = 30.0f;
        projectileResistanceDamage = 0.0f;
        projectileLifetime = 1.0f;
        cadence = 1.0f;
        burstLenght = 4;
        fullBurstDelay = 0.1f;
        break;
    default:
        Console::Log("Burst gun level can't be different from 0, 1, 2 or 3.");
        break;
    }
}