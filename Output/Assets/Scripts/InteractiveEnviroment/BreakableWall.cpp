#include "BreakableWall.h"
#include "../Shooting/Projectile.h"
HELLO_ENGINE_API_C BreakableWall* CreateBreakableWall(ScriptToInspectorInterface* script)
{
    BreakableWall* classInstance = new BreakableWall();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxParticleSystem("BWall Destroyed Particle", &classInstance->wallDestroyed);
    script->AddDragInt("MaxHp", &classInstance->maxHp);
    return classInstance;
}

void BreakableWall::Start()
{
    currentHp = maxHp;
}

void BreakableWall::Update()
{

}

void BreakableWall::OnCollisionEnter(API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();

    if (detectionName == "Projectile")
    {
        Projectile* projectile = (Projectile*)other.GetGameObject().GetScript("Projectile");
        ShootWall(projectile->damage);

    }
}

void BreakableWall::ShootWall(float projectileDamage)
{
    // Health damage
    currentHp -= projectileDamage;
    if (currentHp <= 0)
    {
        currentHp = 0;
        DestroyWall();

        wallDestroyed.Play();
    }
}

void BreakableWall::DestroyWall()
{
    gameObject.SetActive(false);
}