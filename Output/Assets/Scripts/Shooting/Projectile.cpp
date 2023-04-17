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
    gameObject.GetTransform().SetPosition(0, 9999, 0);
    gameObject.SetActive(false);
}

void Projectile::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    
    switch (type)
    {
    case PROJECTILE_TYPE::NONE:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy")
        {
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            switch (action)
            {
            case PROJECTILE_ACTION::SLOW:
                if (enemy) enemy->ActiveSlow(0.5f, 2);
                break;
            case PROJECTILE_ACTION::FREEZE:
                if (enemy) enemy->ActiveStun(2);
                break;
            default:
                break;
            }
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
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 1.0f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, 30.0f, other.GetGameObject().GetUID());
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 1.0f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, 0.0f, other.GetGameObject().GetUID());
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 1.0f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, -30.0f, other.GetGameObject().GetUID());
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::SECONDARY_SEMI:
        if (detectionTag == "Wall" || (detectionTag == "Enemy" && other.GetGameObject().GetUID() != ignoreGO))
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
            pull->LauchProjectileSHOTGUN_BOMB(1.0f, gameObject.GetTransform(), { 0.3f, 0.3f, 0.3f }, other.GetGameObject().GetUID());
            pull->LauchProjectileSHOTGUN_BOMB(1.0f, gameObject.GetTransform(), { 0.3f, 0.3f, 0.3f }, other.GetGameObject().GetUID());
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
            uint exceptionIndex = pull->GetFirstEmptyElectricityChainExeption();
            pull->electricityChainExeptions[exceptionIndex].push_back(other.GetGameObject().GetUID());
            pull->electricityChainExeptionsAmountActive[exceptionIndex]++;
            pull->LauchELECTRICITY_CHAIN(ELECTRICITY_DELAY, 5.0f, 2.0f, other.GetGameObject(), exceptionIndex);
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::FLAMETHROWER:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::RICOCHET:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy")
        {
            if (targetsHitted == 0)
            {
                // do damage
                API_Vector3 rotation = pull->CheckTargetDirectionRICOCHET(gameObject.GetTransform().GetGlobalPosition(), ricochetTarget, other.GetGameObject().GetUID());
                if (rotation.x == 0 && rotation.z == 0) // means enemy in range
                {
                    gameObject.GetTransform().SetRotation(rotation);
                    targetsHitted++;
                }
                else
                {
                    Destroy();
                }
            }
            else if (other.GetGameObject().GetUID() == ricochetTarget)
            {
                if (targetsHitted > 3)
                {
                    Destroy();
                }
                else
                {
                    // do damage
                    API_Vector3 rotation = pull->CheckTargetDirectionRICOCHET(gameObject.GetTransform().GetGlobalPosition(), ricochetTarget, other.GetGameObject().GetUID());
                    if (rotation.x == 0 && rotation.z == 0) // means enemy in range
                    {
                        gameObject.GetTransform().SetRotation(rotation);
                        targetsHitted++;
                    }
                    else
                    {
                        Destroy();
                    }
                }
            }
            else
            {
                // do damage
            }
        }
        break;
    case PROJECTILE_TYPE::PULSE:
        break;
    default:
        break;
    }
}