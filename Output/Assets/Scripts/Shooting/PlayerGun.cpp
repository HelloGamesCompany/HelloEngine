#include "PlayerGun.h"
HELLO_ENGINE_API_C PlayerGun* CreatePlayerGun(ScriptToInspectorInterface* script)
{
    PlayerGun* classInstance = new PlayerGun();
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
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->player);
    script->AddDragInt("Ammo Type", &classInstance->ammoType);
    script->AddInputBox("Audio Event String", &classInstance->audioEventString);
    return classInstance;
}

void PlayerGun::Start()
{
    playerStats = (PlayerStats*)player.GetScript("PlayerStats");
}

void PlayerGun::Update()
{

}

void PlayerGun::Shoot()
{

}

void PlayerGun::EnableGuns(bool enable)
{
    
}

void PlayerGun::SetGunStatsPerLevel(int level)
{

}

void PlayerGun::LauchProjectile(API_Transform projectileSpawn, PROJECTILE_ACTION projectileAction, float randomDirectionRange)
{
    ProjectilePull* pull = (ProjectilePull*)projectilePull.GetScript("ProjectilePull");
    if (pull == nullptr)
    {
        Console::Log("ProjectilePull not asigned");
        return;
    }
    pull->LauchProjectile(projectileSpeed, projectileDamage, projectileResistanceDamage, projectileLifetime, projectileSpawn, projectileMesh, projectileMaterial, projectileScale, projectileAction, randomDirectionRange);
}

void PlayerGun::PlayShotSound(std::string eventString)
{
    Audio::Event(eventString.c_str());
}