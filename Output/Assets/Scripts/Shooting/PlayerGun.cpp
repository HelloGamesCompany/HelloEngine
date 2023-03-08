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
    script->AddDragBoxMeshRenderer("Projectile Mesh", &classInstance->projectileMesh);
    script->AddDragFloat("Projectiles per second", &classInstance->cadence);
    return classInstance;
}

void PlayerGun::Start()
{

}

void PlayerGun::Update()
{

}

void PlayerGun::Shoot()
{

}

void PlayerGun::LauchProjectile()
{
    float x = shootingSpawn.GetUp().x;
    float z = shootingSpawn.GetUp().z;

    PlayerGamepadMovement* playerMovement = (PlayerGamepadMovement*)playerGO.GetScript("PlayerGamepadMovement");
    float aimAngle = playerMovement->_angle;
    API_Vector3 direction = playerGO.GetTransform().GetForward();//API_Vector3(-x, 0, -z);

    ProjectilePull* pull = (ProjectilePull*)projectilePull.GetScript("ProjectilePull");
    if (pull == nullptr)
    {
        Console::Log("ProjectilePull not asigned");
        return;
    }
    pull->LauchProjectile(projectileSpeed, projectileDamage, projectileResistanceDamage, projectileLifetime, aimAngle, shootingSpawn, projectileMesh);
}