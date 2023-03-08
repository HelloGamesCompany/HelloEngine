#include "ProjectilePull.h"
HELLO_ENGINE_API_C ProjectilePull* CreateProjectilePull(ScriptToInspectorInterface* script)
{
    ProjectilePull* classInstance = new ProjectilePull();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Bullet", &classInstance->projectileGO);
    script->AddDragInt("Pull Size", &classInstance->pullSize);
    script->AddDragBoxMeshRenderer("Projectile Mesh", &classInstance->mesh);
    script->AddDragBoxGameObject("Player", &classInstance->playerGO);
    return classInstance;
}

void ProjectilePull::Start()
{
    //for (size_t i = 0; i < pullSize; i++)
    //{
    //    API_GameObject newProjectile = Game::CreateGameObject("Projectile", "Projectile");
    //    newProjectile.AddMeshRenderer(mesh);
    //    //newProjectile.RigidBody
    //    newProjectile.AddScript("Projectile");
    //    newProjectile.GetTransform().SetScale(0.1f, 0.1f, 0.1f);
    //    pull.push_back(newProjectile);
    //}
}

void ProjectilePull::Update()
{
    
}

API_GameObject ProjectilePull::GetFirstActiveProjectile()
{
    /*for (size_t i = 0; i < 5; i++)
    {
        if (!pull.at(i).active)) return pull.at(i);
    }*/

    return pull.at(0);
}

void ProjectilePull::LauchProjectile(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_MeshRenderer projectileMesh)
{
    API_GameObject go = projectileGO;//GetFirstActiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetLocalRotation());
    //go.ChangeMesh;

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    //projectile->direction = direction;
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
}