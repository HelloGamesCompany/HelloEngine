#include "EnemyMeleeSpawner.h"
#include "../Enemies/EnemyMeleeMovement.h"
#include "../Enemies/Enemy.h"
#include "../Shooting/Projectile.h"
#include "API/API_AnimationPlayer.h"

HELLO_ENGINE_API_C EnemyMeleeSpawner* CreateEnemyMeleeSpawner(ScriptToInspectorInterface* script)
{
    EnemyMeleeSpawner* classInstance = new EnemyMeleeSpawner();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

    script->AddDragBoxPrefabResource("Enemy Prefab", &classInstance->enemyPrefabRes);
    script->AddDragBoxPrefabResource("Spawn Destroyed", &classInstance->spawnDestroyedRes);
    script->AddDragBoxGameObject("Spawn Entire", &classInstance->spawnEntireRef);
    script->AddDragBoxGameObject(" Parent of the Spawn", &classInstance->parentOfSpawn);
    script->AddDragBoxGameObject(" Parent of the Pool", &classInstance->parent);
    script->AddDragInt("Enemy Pool Size", &classInstance->spawnPoolSize);
    script->AddDragBoxParticleSystem("Enemy Spawn Particles", &classInstance->enemySpawnParticle);
    script->AddDragBoxParticleSystem("Spawner Destroyed Particles", &classInstance->spawnerDestroyedParticle);
    script->AddDragBoxGameObject("Target", &classInstance->target);
    script->AddDragBoxGameObject("Action zone", &classInstance->actionZone);
    script->AddDragBoxRigidBody("Action Rb zone", &classInstance->zoneRb);
    script->AddDragBoxGameObject("Point 1", &classInstance->listPoints[0]);
    script->AddDragBoxGameObject("Point 2", &classInstance->listPoints[1]);
    script->AddDragBoxGameObject("Point 3", &classInstance->listPoints[2]);

    return classInstance;
}

void EnemyMeleeSpawner::Start()
{

    enemiesInSpawn.resize(spawnPoolSize);

    spawnDestroyedRef = Game::InstancePrefab(spawnDestroyedRes, parentOfSpawn);
    spawnDestroyedRef.GetTransform().SetPosition(spawnEntireRef.GetTransform().GetLocalPosition());
    spawnDestroyedRef.GetTransform().SetRotation(spawnEntireRef.GetTransform().GetLocalRotation());
    spawnDestroyedRef.GetTransform().SetScale(spawnEntireRef.GetTransform().GetLocalScale());
    spawnDestroyedRef.SetActive(false);

    for (size_t i = 0; i < spawnPoolSize; i++)
    {

        API_GameObject rootBoneRef;
        API_SkinnedMeshRenderer skinnedMeshRef;
        API_GameObject attackZoneRef;

        std::string detectionTag;

        enemiesInSpawn[i] = Game::InstancePrefab(enemyPrefabRes, parent);

        enemiesInSpawn[i].GetChildren(childs);

        for (API_GameObject& var : childs)
        {
            if (!var.IsAlive())
            {
                continue;
            }

            detectionTag = var.GetTag();

            if (detectionTag == "SkinnedMesh")
            {
                skinnedMeshRef = var.GetSkinnedMeshRender();
            }
            else if (detectionTag == "RootBone")
            {
                rootBoneRef = var;
            }
            else if (detectionTag == "AttackZone")
            {
                attackZoneRef = var;
            }
        }

        skinnedMeshRef.SetRootBone(rootBoneRef);

        Enemy* enemyScript = (Enemy*)enemiesInSpawn[i].GetScript("Enemy");
        if (enemyScript != nullptr)
        {
            enemyScript->enemyRb = enemiesInSpawn[i].GetRigidBody();

            enemyScript->Start();
        }

        EnemyMeleeMovement* enemyMeleeMov = (EnemyMeleeMovement*)enemiesInSpawn[i].GetScript("EnemyMeleeMovement");
        if (enemyMeleeMov != nullptr)
        {
             
            enemyMeleeMov->target = target;
            enemyMeleeMov->actionZone = actionZone;
            enemyMeleeMov->zoneRb = zoneRb;
            enemyMeleeMov->listPoints[0] = listPoints[0];
            enemyMeleeMov->listPoints[1] = listPoints[1];
            enemyMeleeMov->listPoints[2] = listPoints[2];
            enemyMeleeMov->attackZoneGO = attackZoneRef;
            enemyMeleeMov->animationPlayer = skinnedMeshRef.GetGameObject().GetAnimationPlayer();

            enemyMeleeMov->Start();
        }

        enemiesInSpawn[i].SetActive(false);
    }
}
void EnemyMeleeSpawner::Update()
{
    if (!destroyed) 
    {
        spawnTimer += Time::GetDeltaTime();

        if (spawnTimer >= 3.0f)
        {

            SpawnEnemy(GetEnemyIndexInactive());

            spawnTimer = 0.0f;

        }
    }
}

void EnemyMeleeSpawner::OnCollisionEnter(API_RigidBody other)
{
    if (!destroyed)
    {
        std::string detectionName = other.GetGameObject().GetName();

        if (detectionName == "Projectile")
        {

            Projectile* projectile = (Projectile*)other.GetGameObject().GetScript("Projectile");

            ShootSpawn(projectile->damage);

        }
        else if (detectionName == "Cube")
        {
            DestroySpawn();
        }
    }
}

void EnemyMeleeSpawner::SpawnEnemy(int i)
{

    if (i == -1)
    {
        Console::Log("NOT SPAWN");
        return;
    }
    Console::Log("YES SPAWN");
    enemiesInSpawn[i].GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition());
    enemiesInSpawn[i].SetActive(true);

}

int EnemyMeleeSpawner::GetEnemyIndexInactive()
{

    for (int i = 0; i < spawnPoolSize; i++)
    {
        if (!enemiesInSpawn[i].IsActive())
        {
            return i;
        }

    }

    return -1;
}

void EnemyMeleeSpawner::ShootSpawn(float projectileDamage)
{
    currentHp -= projectileDamage;
    if (currentHp <= 0)
    {
        currentHp = 0;
        DestroySpawn();

        //spawnerDestroyedParticle.Play();

    }
}

void EnemyMeleeSpawner::DestroySpawn()
{
    Audio::Event("spawn_breaking");
    gameObject.SetActive(false);
    spawnDestroyedRef.SetActive(true);
    destroyed = true;
}
