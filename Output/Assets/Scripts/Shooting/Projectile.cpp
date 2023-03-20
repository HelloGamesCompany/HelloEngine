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
    //particles.Stop();
    gameObject.SetActive(false);
}

void Projectile::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    switch (action)
    {
    case PROJECTILE_ACTION::NONE:
        if (detectionName != "Player" && detectionName != "Projectile")
        {
            Destroy();
        }
        break;
    case PROJECTILE_ACTION::FLAMETROWER:
        break;
    case PROJECTILE_ACTION::RICOCHET:
        if (detectionName == "Wall" && wallCd <= 0)
        {
            gameObject.GetTransform().Rotate(0, 180, 0);
            wallCd = 1;
        }
        break;
    default:
        break;
    }
}