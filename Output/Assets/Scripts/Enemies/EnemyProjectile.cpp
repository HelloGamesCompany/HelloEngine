#include "EnemyProjectile.h"
HELLO_ENGINE_API_C EnemyProjectile* CreateEnemyProjectile(ScriptToInspectorInterface* script)
{
	EnemyProjectile* classInstance = new EnemyProjectile();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void EnemyProjectile::Start()
{

}

void EnemyProjectile::Update()
{
    lifeTime -= Time::GetDeltaTime();

    if (lifeTime <= 0)
    {
        Destroy();
        return;
    }

    gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * speed * Time::GetDeltaTime());
}

void EnemyProjectile::Destroy()
{
    gameObject.GetParticleSystem().Stop();
    gameObject.SetActive(false);
}

void EnemyProjectile::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    if (detectionName != "EnemyR" && detectionName != "Projectile" && detectionName != "Zone")
    {
        Destroy();
    }
}