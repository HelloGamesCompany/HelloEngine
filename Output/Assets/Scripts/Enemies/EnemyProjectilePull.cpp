#include "EnemyProjectilePull.h"
HELLO_ENGINE_API_C EnemyProjectilePull* CreateEnemyProjectilePull(ScriptToInspectorInterface* script)
{
	EnemyProjectilePull* classInstance = new EnemyProjectilePull();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	
    script->AddDragInt("Pull Size", &classInstance->pullSize);
    script->AddDragBoxGameObject("Enemy", &classInstance->enemyGO);
    script->AddDragBoxParticleSystem("Particles", &classInstance->particleTest);

    return classInstance;
}

void EnemyProjectilePull::Start()
{
    srand(time(NULL));

    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newProjectile = Game::CreateGameObject("Projectile", "Projectile");
        newProjectile.AddMeshRenderer();
        newProjectile.AddMaterial();
        newProjectile.CreateRigidBodyBox((0, 0, 0), (0, 0, 0), (0.3f, 0.3f, 0.3f), false);
        newProjectile.AddParticleSystem(particleTest);
        newProjectile.AddScript("Projectile");
        newProjectile.SetActive(false);
        pull.push_back(newProjectile);
    }
}

void EnemyProjectilePull::Update()
{

}

API_GameObject EnemyProjectilePull::GetFirstActiveProjectile()
{
    for (size_t i = 0; i < pullSize; i++)
    {
        if (!pull[i].IsActive()) return pull[i];
    }

    return pull[0];
}

void EnemyProjectilePull::LauchProjectile(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, uint projectileMesh, uint projectileMaterial, API_Vector3 projectileScale  , bool randomDirection)
{
    API_GameObject go = GetFirstActiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(enemyGO.GetTransform().GetLocalRotation());
    go.GetTransform().SetScale(projectileScale);
    
        go.GetMeshRenderer().ChangeMesh(projectileMesh);
        go.GetMaterialCompoennt().ChangeAlbedoTexture(projectileMaterial);
        go.GetParticleSystem().Play();
    

    if (randomDirection)
    {
        float offsetX = (-49 + rand() % (100)) / 5.0f; // values between -25 and 25
        float offsetY = (-49 + rand() % (100)) / 5.0f;
        go.GetTransform().Rotate(offsetX, offsetY, 0);
    }

    EnemyProjectile* projectile = (EnemyProjectile*)go.GetScript("EnemyProjectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
   

    Audio::Event("basic_shooting");
}