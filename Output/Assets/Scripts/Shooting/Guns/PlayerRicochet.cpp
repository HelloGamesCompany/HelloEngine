#include "PlayerRicochet.h"
HELLO_ENGINE_API_C PlayerRicochet* CreatePlayerRicochet(ScriptToInspectorInterface* script)
{
    PlayerRicochet* classInstance = new PlayerRicochet();
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
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->player);
    script->AddDragInt("Ammo Type", &classInstance->ammoType);
    script->AddInputBox("Audio Event String", &classInstance->audioEventString);
    return classInstance;
}

void PlayerRicochet::Start()
{
    playerStats = (PlayerStats*)player.GetScript("PlayerStats");

    SetGunStatsPerLevel(API_QuickSave::GetInt("ricochet_level")); // read from save file

    if (cadence == 0)
    {
        fullShotCooldown = 0;
        fullShotCooldownWithPowerUp = 0;
    }
    else
    {
        fullShotCooldown = 1 / cadence;
        fullShotCooldownWithPowerUp = 1 / (cadence * 1.5f); // 50% increase
    }
}

void PlayerRicochet::Update()
{
    float dt;
    if (playerStats->slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    if (shotBuffer)
    {
        shotBufferCooldown -= Time::GetRealTimeDeltaTime();
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
        shotCooldown -= dt;
    }
}

void PlayerRicochet::Shoot()
{
    if (canShoot)
    {
        LauchProjectile(shootingSpawn, PROJECTILE_TYPE::RICOCHET);
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

void PlayerRicochet::EnableGuns(bool enable)
{
    gameObject.SetActive(enable);
    shotBuffer = false;
}

void PlayerRicochet::SetGunStatsPerLevel(int level)
{
    switch (level)
    {
    case 0:
        projectileSpeed = 30.0f;
        projectileDamage = 5.0f;
        projectileResistanceDamage = 0.0f;
        projectileLifetime = 10.0f;
        cadence = 2.0f;
        break;
    case 1:
        projectileSpeed = 30.0f;
        projectileDamage = 5.0f;
        projectileResistanceDamage = 0.0f;
        projectileLifetime = 1.0f;
        cadence = 4.0f;
        break;
    case 2:
        projectileSpeed = 45.0f;
        projectileDamage = 5.0f;
        projectileResistanceDamage = 0.0f;
        projectileLifetime = 1.0f;
        cadence = 4.0f;
        break;
    case 3:
        projectileSpeed = 45.0f;
        projectileDamage = 15.0f;
        projectileResistanceDamage = 0.0f;
        projectileLifetime = 1.0f;
        cadence = 4.0f;
        break;
    default:
        Console::Log("Automatic gun level can't be different from 0, 1, 2 or 3.");
        break;
    }
}