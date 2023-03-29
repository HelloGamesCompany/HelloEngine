#include "EnemyGun.h"
HELLO_ENGINE_API_C EnemyGun* CreateEnemyGun(ScriptToInspectorInterface* script)
{
    EnemyGun* classInstance = new EnemyGun();
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
    script->AddInputBox("Audio Event String", &classInstance->audioEventString);
    return classInstance;
}

void EnemyGun::Start()
{

}
void EnemyGun::Update()
{

}

void EnemyGun::Shoot()
{

}

void EnemyGun::LauchProjectile(API_Transform projectileSpawn, float randomDirectionRange)
{
    EnemyProjectilePull* pull = (EnemyProjectilePull*)projectilePull.GetScript("EnemyProjectilePull");
    if (pull == nullptr)
    {
        Console::Log("EnemyProjectilePull not asigned");
        return;
    }
    pull->LauchProjectile(projectileSpeed, projectileDamage, projectileResistanceDamage, projectileLifetime, projectileSpawn, projectileMesh, projectileMaterial, enemyGO.GetTransform().GetLocalRotation(), projectileScale, randomDirectionRange);
}

void EnemyGun::PlayShotSound(std::string eventString)
{
    Audio::Event(eventString.c_str());
}