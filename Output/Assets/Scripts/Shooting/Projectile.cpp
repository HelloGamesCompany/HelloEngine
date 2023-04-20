#include "Projectile.h"
#include "ProjectilePull.h"
#include "../Enemies/Enemy.h"
#include "../EbonyMaw/BossLoop.h"
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
            if (enemy)
            {
                enemy->TakeDamage(damage, resistanceDamage);
                enemy->CheckBombs();
            }
            Destroy();
        }
        else if (detectionTag == "Boss")
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss)
            {
                miniBoss->TakeDamage(damage);
                miniBoss->CheckBombs();
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
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            if (enemy) enemy->TakeDamage(damage, resistanceDamage);
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 1.0f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, 30.0f, other.GetGameObject().GetUID());
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 1.0f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, 0.0f, other.GetGameObject().GetUID());
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 1.0f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, -30.0f, other.GetGameObject().GetUID());
            Destroy();
        }
        else if (detectionTag == "Boss") // EXIT
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss) miniBoss->TakeDamage(damage);
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 0.5f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, 30.0f, other.GetGameObject().GetUID());
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 0.5f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, 0.0f, other.GetGameObject().GetUID());
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 0.5f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, -30.0f, other.GetGameObject().GetUID());
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::SECONDARY_SEMI:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy" && other.GetGameObject().GetUID() != ignoreGO)
        {
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            if (enemy) enemy->TakeDamage(damage, resistanceDamage);
            Destroy();
        }
        else if (detectionTag == "Boss" && other.GetGameObject().GetUID() != ignoreGO)
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss) miniBoss->TakeDamage(damage);
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::AUTO:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        if (detectionTag == "Enemy")
        {
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            if (enemy) enemy->TakeDamage(damage, resistanceDamage);
            Destroy();
        }
        if (detectionTag == "Boss")
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss) miniBoss->TakeDamage(damage);
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::BURST:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy")
        {
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            if (enemy) enemy->AddBomb();
            Destroy();
        }
        else if (detectionTag == "Boss")
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss) miniBoss->AddBomb();
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
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            if (enemy) enemy->TakeDamage(damage, resistanceDamage);
            pull->LauchProjectileSHOTGUN_BOMB(0.5f, gameObject.GetTransform(), { 0.3f, 0.3f, 0.3f }, other.GetGameObject().GetUID());
            pull->LauchProjectileSHOTGUN_BOMB(0.5f, gameObject.GetTransform(), { 0.3f, 0.3f, 0.3f }, other.GetGameObject().GetUID());
            Destroy();
        }
        else if (detectionTag == "Boss") // EXIT
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss) miniBoss->TakeDamage(damage);
            pull->LauchProjectileSHOTGUN_BOMB(0.5f, gameObject.GetTransform(), { 0.3f, 0.3f, 0.3f }, other.GetGameObject().GetUID());
            pull->LauchProjectileSHOTGUN_BOMB(0.5f, gameObject.GetTransform(), { 0.3f, 0.3f, 0.3f }, other.GetGameObject().GetUID());
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
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            if (enemy) enemy->TakeDamage(damage, resistanceDamage);
            uint exceptionIndex = pull->GetFirstEmptyElectricityChainExeption();
            pull->electricityChainExeptions[exceptionIndex].push_back(other.GetGameObject().GetUID());
            pull->electricityChainExeptionsAmountActive[exceptionIndex]++;
            pull->LauchELECTRICITY_CHAIN(ELECTRICITY_DELAY, 5.0f, 2.0f, other.GetGameObject(), exceptionIndex);
            Destroy();
        }
        else if (detectionTag == "Boss")
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss) miniBoss->TakeDamage(damage);
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
        else if (detectionTag == "Enemy")
        {
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            if (enemy)
            {
                enemy->TakeDamage(damage, resistanceDamage);
                enemy->CheckBombs();
            }
            Destroy();
        }
        else if (detectionTag == "Boss")
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss)
            {
                miniBoss->TakeDamage(damage);
                miniBoss->CheckBombs();
                miniBoss->AddBurn();
            }
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::RICOCHET:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy" || detectionTag == "Boss")
        {
            if (targetsHitted == 0)
            {
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
            if (detectionTag == "Enemy")
            {
                Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
                if (enemy)
                {
                    enemy->TakeDamage(damage, resistanceDamage);
                    enemy->CheckBombs();
                }
            }
            else if (detectionTag == "Boss")
            {
                BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
                if (miniBoss)
                {
                    miniBoss->TakeDamage(damage);
                    miniBoss->CheckBombs();
                }
            }
        }
        break;
    case PROJECTILE_TYPE::PULSE:
        break;
    default:
        break;
    }
}