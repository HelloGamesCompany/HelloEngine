#include "ProjectilePull.h"
#include <time.h>
HELLO_ENGINE_API_C ProjectilePull* CreateProjectilePull(ScriptToInspectorInterface* script)
{
    ProjectilePull* classInstance = new ProjectilePull();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragInt("Pull Size", &classInstance->pullSize);
    script->AddDragBoxGameObject("Player", &classInstance->playerGO);
    script->AddDragBoxParticleSystem("Particles", &classInstance->particleTest);
    return classInstance;
}

void ProjectilePull::Start()
{
    srand(time(NULL));

    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newProjectile = Game::CreateGameObject("Projectile", "Projectile");
        newProjectile.AddMeshRenderer();
        newProjectile.AddMaterial();
        API_RigidBody rb = newProjectile.CreateRigidBodyBox((0, 0, 0), (0, 0, 0), (0.3f, 0.3f, 0.3f), false);
        rb.SetTrigger(true);
        newProjectile.AddParticleSystem(particleTest);
        newProjectile.AddScript("Projectile");
        newProjectile.SetActive(false);
        pull.push_back(newProjectile);
    }
}

void ProjectilePull::Update()
{
    
}

API_GameObject ProjectilePull::GetFirstInactiveProjectile()
{
    for (size_t i = 0; i < pullSize; i++)
    {
        if (!pull[i].IsActive()) return pull[i];
    }

    return pull[0];
}

void ProjectilePull::LauchProjectile(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, uint projectileMesh, uint projectileMaterial, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction, float randomDirectionRange)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    if (projectileAction == PROJECTILE_ACTION::FLAMETROWER)
    {
        // hide projectile render
    }
    else
    {
        go.GetMeshRenderer().ChangeMesh(projectileMesh);
        go.GetMaterialCompoennt().ChangeAlbedoTexture(projectileMaterial);
        /*go.GetParticleSystem().Play();*/
    }

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
    projectile->action = projectileAction;
}