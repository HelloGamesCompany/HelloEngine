#include "Projectile.h"
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
        //Destroy();
        return;
    }

    //gameObject.GetTransform().Translate(direction);
    //gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward());
}

void Projectile::Destroy()
{
    gameObject.SetActive(false);
}

void Projectile::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    if (detectionName != "Player")
    {
        gameObject.SetActive(false);
    }
}