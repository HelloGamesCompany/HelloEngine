#include "ProjectilePull.h"
#include "../Player/PlayerStats.h"
#include "ShotgunBombExplosion.h"
#include "ElectricityChain.h"
#include "CheckRicochetTargets.h"
#include <time.h>
HELLO_ENGINE_API_C ProjectilePull* CreateProjectilePull(ScriptToInspectorInterface* script)
{
    ProjectilePull* classInstance = new ProjectilePull();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player", &classInstance->playerGO);
    script->AddDragInt("Pull Size", &classInstance->pullSize);
    script->AddDragBoxPrefabResource("Projectile Prefab", &classInstance->projectilePrefab);
    script->AddDragInt("Shotgun Bomb Pull Size", &classInstance->shotgunBombPullSize);
    script->AddDragBoxPrefabResource("Shotgun Bomb Prefab", &classInstance->shotgunBombPrefab);
    script->AddDragInt("Electricity Chain Pull Size", &classInstance->electricityChainPullSize);
    script->AddDragBoxPrefabResource("Electricity Chain Prefab", &classInstance->electricityChainPrefab);
    script->AddDragBoxPrefabResource("Check Ricochet Traget Prefab", &classInstance->checkRicochetTargetsPrefab);
    return classInstance;
}

void ProjectilePull::Start()
{
    srand(time(NULL));

    playerStats = (PlayerStats*)playerGO.GetScript("PlayerStats");
    if (playerStats == nullptr) Console::Log("Missing PlayerStats on ProjectilePull Script.");

    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newProjectile = Game::InstancePrefab(projectilePrefab, API_GameObject());
        //all start
        Projectile* projectileScript = (Projectile*)newProjectile.GetScript("Projectile");
        projectileScript->pull = this;
        pull.push_back(newProjectile);
    }

    for (size_t i = 0; i < shotgunBombPullSize; i++)
    {
        API_GameObject newBomb = Game::InstancePrefab(shotgunBombPrefab, API_GameObject());
        //all start
        ShotgunBomb* bombScript = (ShotgunBomb*)newBomb.GetScript("ShotgunBomb");
        API_GameObject explosion[1];
        newBomb.GetChildren(explosion);
        bombScript->explosion = (ShotgunBombExplosion*)explosion->GetScript("ShotgunBombExplosion");
        bombScript->explosion->shotgunBomb = newBomb;
        shotgunBombPull.push_back(newBomb);
    }

    for (size_t i = 0; i < electricityChainPullSize; i++)
    {
        API_GameObject newChain = Game::InstancePrefab(electricityChainPrefab, API_GameObject());
        //all start
        ElectricityChain* electricityChain = (ElectricityChain*)newChain.GetScript("ElectricityChain");
        electricityChain->pull = this;
        electricityChainPull.push_back(newChain);
        std::vector<uint> vector;
        electricityChainExeptions.push_back(vector);
        electricityChainExeptionsAmountActive.push_back(0);
    }

    ricochetDetector = Game::InstancePrefab(checkRicochetTargetsPrefab, API_GameObject());
    ricochetTargets = (CheckRicochetTargets*)ricochetDetector.GetScript("CheckRicochetTargets");
    if (ricochetTargets == nullptr) Console::Log("Missing CheckRicochetTargets on ProjectilePull Script.");
}

void ProjectilePull::Update()
{
    float dt;
    if (playerStats && playerStats->slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    Console::Log(std::to_string(1 + (autoForce * 4.8f)));
    if (resetAuto >= 0.0f && autoForce >= 0.0f)
    {
        resetAuto -= dt;
        if (resetAuto <= 0.0f)
        {
            resetAuto = 0.0f;
            autoForce -= dt * 1.0f;
        }
    }
}

API_GameObject ProjectilePull::GetFirstInactiveProjectile()
{
    for (size_t i = 0; i < pullSize; i++)
    {
        if (!pull[i].IsActive()) return pull[i];
    }

    return pull[0];
}

API_GameObject ProjectilePull::GetFirstInactiveShotgunBomb()
{
    for (size_t i = 0; i < shotgunBombPullSize; i++)
    {
        if (!shotgunBombPull[i].IsActive()) return shotgunBombPull[i];
    }

    return shotgunBombPull[0];
}

API_GameObject ProjectilePull::GetFirstInactiveElectricityChain()
{
    for (size_t i = 0; i < electricityChainPullSize; i++)
    {
        if (!electricityChainPull[i].IsActive()) return electricityChainPull[i];
    }

    return electricityChainPull[0];
}

uint ProjectilePull::GetFirstEmptyElectricityChainExeption()
{
    for (size_t i = 0; i < electricityChainExeptions.size(); i++)
    {
        if (electricityChainExeptionsAmountActive[i] == 0)
        {
            electricityChainExeptions[i].clear();
            return i;
        }
    }

    return 0;
}

void ProjectilePull::LauchProjectileNORMAL(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();
    go.GetMeshRenderer().SetActive(true);

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->action = projectileAction;
    projectile->type = PROJECTILE_TYPE::NONE;
    projectile->ignoreGO = 0;
}

void ProjectilePull::LauchProjectileSEMI(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();
    go.GetMeshRenderer().SetActive(true);

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->type = PROJECTILE_TYPE::SEMI;
    projectile->ignoreGO = 0;
}

void ProjectilePull::LauchProjectileSECONDARY_SEMI(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, float rotateY, uint ignoreGO)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().Rotate(0, rotateY, 0);
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();
    go.GetMeshRenderer().SetActive(true);

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->type = PROJECTILE_TYPE::SECONDARY_SEMI;
    projectile->ignoreGO = ignoreGO;
}

void ProjectilePull::LauchProjectileAUTO(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();
    go.GetMeshRenderer().SetActive(true);

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage + (autoForce * 4.8f);
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->type = PROJECTILE_TYPE::AUTO;
    projectile->ignoreGO = 0;

    if (autoForce <= 6.0f) autoForce += 0.1f;
    resetAuto = 0.2f;
}

void ProjectilePull::LauchProjectileBURST(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();
    go.GetMeshRenderer().SetActive(true);

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->type = PROJECTILE_TYPE::BURST;
    projectile->ignoreGO = 0;
}

void ProjectilePull::LauchProjectileSHOTGUN(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, float randomDirectionRange)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();
    go.GetMeshRenderer().SetActive(true);

    if (randomDirectionRange > 0.0f)
    {
        float offsetX = (-49 + rand() % (100)) * randomDirectionRange;
        float offsetY = (-49 + rand() % (100)) * randomDirectionRange;
        go.GetTransform().Rotate(offsetX, offsetY, 0);
    }

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->type = PROJECTILE_TYPE::SHOTGUN;
    projectile->ignoreGO = 0;
}

void ProjectilePull::LauchProjectileSHOTGUN_BOMB(float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, uint ignoreGO)
{
    API_GameObject go = GetFirstInactiveShotgunBomb();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();
    go.GetMeshRenderer().SetActive(true);

    float offsetY = rand() % 360;
    go.GetTransform().Rotate(0, offsetY, 0);

    ShotgunBomb* bomb = (ShotgunBomb*)go.GetScript("ShotgunBomb");
    bomb->lifeTime = projectileLifetime;
    bomb->ResetExposion();
    bomb->ignoreGO = ignoreGO;
}

void ProjectilePull::LauchProjectileHANDGUN(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();
    go.GetMeshRenderer().SetActive(true);

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->type = PROJECTILE_TYPE::HANDGUN;
    projectile->ignoreGO = 0;
}

void ProjectilePull::LauchELECTRICITY_CHAIN(float delay, float damage, float resistanceDamage, API_GameObject attachedToGO, uint exceptionsVectorIndex)
{
    API_GameObject go = GetFirstInactiveElectricityChain();
    go.SetActive(true);
    //go.GetParticleSystem().Play();

    ElectricityChain* electricityChain = (ElectricityChain*)go.GetScript("ElectricityChain");
    electricityChain->delay = delay;
    electricityChain->damage = damage;
    electricityChain->resistanceDamage = resistanceDamage;
    electricityChain->atachedToGO = attachedToGO;
    electricityChain->exeptionsVectorIndex = exceptionsVectorIndex;
    electricityChain->triggerActive = false;
    electricityChain->chainCount = 0;
    electricityChain->destroy = false;
}

void ProjectilePull::LauchProjectileFLAMETHROWER(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    //go.GetMeshRenderer().SetActive(false);

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->type = PROJECTILE_TYPE::FLAMETHROWER;
    projectile->ignoreGO = 0;
}

void ProjectilePull::LauchProjectileRICOCHET(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();
    go.GetMeshRenderer().SetActive(true);

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->targetsHitted = 0;
    projectile->type = PROJECTILE_TYPE::RICOCHET;
    projectile->ignoreGO = 0;

    ricochetDetector.SetActive(true);
    ricochetTargets->atachedToGO = go;
}

API_Vector3 ProjectilePull::CheckTargetDirectionRICOCHET(API_Vector3 ricochetPos, uint& targetUID, uint ignoreGO)
{
    if (!ricochetTargets) return {1, 0, 1};
    API_GameObject go;
    do
    {
        go = ricochetTargets->GetRandomTarget();
    } while (ignoreGO == go.GetUID());
    std::string compareTag = go.GetTag();
    if (compareTag != "Enemy" && compareTag != "Boss") return { 1, 0, 1 };
    
    float angleY = atan2((double)ricochetPos.z - (double)go.GetTransform().GetGlobalPosition().z, -(double)ricochetPos.x + (double)go.GetTransform().GetGlobalPosition().x);
    targetUID = go.GetUID();
    return { 0, angleY * (180.0f / 3.141592653589793238463f) + 90, 0 };
}