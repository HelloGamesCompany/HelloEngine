#include "BreakableWall.h"
#include "../Shooting/Projectile.h"
#include "../Player/PlayerMove.h"
HELLO_ENGINE_API_C BreakableWall* CreateBreakableWall(ScriptToInspectorInterface* script)
{
    BreakableWall* classInstance = new BreakableWall();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxParticleSystem("Wall Destroyed Particle", &classInstance->wallDestroyed);
    script->AddDragInt("MaxHp", &classInstance->maxHp);
    script->AddDragBoxGameObject("Fence Destroyed", &classInstance->fenceDestroyed);
    script->AddDragBoxGameObject("Fence Entire", &classInstance->fenceEntire);
    script->AddDragBoxRigidBody("Fence Entire Rb", &classInstance->fenceRigidbody);

    return classInstance;
}

void BreakableWall::Start()
{
    currentHp = maxHp;

    fenceDestroyed.SetActive(false);
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
    else if (detectionName == "Player")
    {
        PlayerMove* playerMove = (PlayerMove*)other.GetGameObject().GetScript("PlayerMove");

        if (playerMove->isDashing)
        {
            DestroyWall();
        }
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

        
    }
}

void BreakableWall::DestroyWall()
{
    wallDestroyed.Play();

    Audio::Event("fence_breaking");

    fenceDestroyed.SetActive(true);
    fenceEntire.SetActive(false);
    fenceRigidbody.SetTrigger(true);
}