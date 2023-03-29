#include "Projectile.h"
#include <iostream>
HELLO_ENGINE_API_C Projectile* CreateProjectile(ScriptToInspectorInterface* script)
{
    Projectile* classInstance = new Projectile();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void Projectile::Start()
{

}

void Projectile::Update()
{
    lifeTime -= Time::GetDeltaTime();
    wallCd -= Time::GetDeltaTime();

    if (lifeTime <= 0)
    {
        Destroy();
        return;
    }

    gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * speed * Time::GetDeltaTime());
}

void Projectile::Destroy()
{
    gameObject.GetParticleSystem().Stop();
    gameObject.SetActive(false);
}

void Projectile::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    
    switch (action)
    {
    case PROJECTILE_ACTION::NONE:
        if (detectionTag == "Wall" || detectionTag == "Enemy")
        {
            Destroy();
        }
        break;
    case PROJECTILE_ACTION::SLOW:
        if (detectionTag == "Wall" || detectionTag == "Enemy")
        {
            // apply slow to enemy
            Destroy();
        }
        break;
    case PROJECTILE_ACTION::FREEZE:
        if (detectionTag == "Wall" || detectionTag == "Enemy")
        {
            // apply freeze to enemy
            Destroy();
        }
        break;
    case PROJECTILE_ACTION::FLAMETROWER:
        break;
    case PROJECTILE_ACTION::RICOCHET:
        if (detectionTag == "Wall" && wallCd <= 0)
        {
            gameObject.GetTransform().Rotate(0, 180, 0);
            wallCd = 1;
        }
        break;
    default:
        break;
    }
}