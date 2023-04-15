#include "Projectile.h"
#include "ProjectilePull.h"
#include "../Enemies/Enemy.h"
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
    
    switch (type)
    {
    case PROJECTILE_TYPE::NONE:
        if (detectionTag == "Wall" || detectionTag == "Enemy")
        {
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::SEMI:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy") // EXIT
        {
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 1.0f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, PROJECTILE_ACTION::NONE, 30.0f);
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 1.0f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, PROJECTILE_ACTION::NONE, -30.0f);
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::SECONDARY_SEMI:
        if (detectionTag == "Wall" || detectionTag == "Enemy")
        {
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::AUTO:
        if (detectionTag == "Wall" || detectionTag == "Enemy")
        {
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::BURST:
        if (detectionTag == "Wall" || detectionTag == "Enemy")
        {
            // enganchar mina
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::SHOTGUN:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy") // EXIT
        {
            pull->LauchProjectileSHOTGUN_BOMB(1.0f, gameObject.GetTransform(), { 0.3f, 0.3f, 0.3f }, PROJECTILE_ACTION::NONE);
            pull->LauchProjectileSHOTGUN_BOMB(1.0f, gameObject.GetTransform(), { 0.3f, 0.3f, 0.3f }, PROJECTILE_ACTION::NONE);
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::HANDGUN:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy")
        {
            pull->LauchProjectileELECTRICITY_CHAIN(ELECTRICITY_DELAY, 5.0f, 2.0f, other.GetGameObject());
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::FLAMETHROWER:
        break;
    case PROJECTILE_TYPE::RICOCHET:
        break;
    case PROJECTILE_TYPE::PULSE:
        break;
    default:
        break;
    }
}