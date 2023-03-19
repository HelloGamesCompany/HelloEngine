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
    script->AddDragBoxMeshResource("Projectile Mesh", &classInstance->projectileMesh);
    script->AddDragBoxTextureResource("Projectile Material", &classInstance->projectileMaterial);
    script->AddDragFloat("Projectile ScaleX", &classInstance->projectileScale.x);
    script->AddDragFloat("Projectile ScaleY", &classInstance->projectileScale.y);
    script->AddDragFloat("Projectile ScaleZ", &classInstance->projectileScale.z);
    script->AddDragFloat("Bursts per second", &classInstance->cadence);
    script->AddDragFloat("Burst Space", &classInstance->fullBurstDelay);
    script->AddDragInt("Projectiles per burst", &classInstance->burstLenght);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->player);
    script->AddDragInt("Ammo Type", &classInstance->ammoType);
    return classInstance;
}

void PlayerBurst::Start()
{
    playerStats = (PlayerStats*)player.GetScript("PlayerStats");

    if (cadence != 0) fullShotCooldown = 1 / cadence;
    else fullShotCooldown = 0;

    shotCount = burstLenght;
}

void PlayerBurst::Update()
{
    // burst
    if (shotCount < burstLenght)
    {
        if (burstDelay <= 0)
        {
            shotCount++;
            burstDelay = fullBurstDelay;
            LauchProjectile(shootingSpawn);
            playerStats->UseAmmo(ammoType);
        }
        else
        {
            burstDelay -= Time::GetDeltaTime();
        }
    }

    if (canShoot)return;

    if (shotCooldown <= 0)
    {
        canShoot = true;
    }
    else
    {
        shotCooldown -= Time::GetDeltaTime();
    }
}

void PlayerBurst::Shoot()
{
    if (canShoot)
    {
        LauchProjectile(shootingSpawn);
        canShoot = false;
        shotCooldown = fullShotCooldown;
        shotCount = 1;
        burstDelay = fullBurstDelay;
        playerStats->UseAmmo(ammoType);
    }
}

void PlayerBurst::EnableGuns(bool enable)
{
    gameObject.SetActive(enable);
}