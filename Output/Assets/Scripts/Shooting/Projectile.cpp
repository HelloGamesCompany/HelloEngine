#include "Projectile.h"
#include <iostream>
HELLO_ENGINE_API_C Projectile* CreateProjectile(ScriptToInspectorInterface* script)
{
    Projectile* classInstance = new Projectile();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Speed", &classInstance->speed);
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

    //float dirX = gameObject.GetTransform().GetForward().x;
    //float dirY = 0.0f;
    //float dirZ = gameObject.GetTransform().GetForward().z;
    std::cout << "go" << std::endl;
    std::cout << "X: " << gameObject.GetTransform().GetLocalPosition().x << "Y: " << gameObject.GetTransform().GetLocalPosition().y << "Z: " << gameObject.GetTransform().GetLocalPosition().z << std::endl;
    // gameObject.GetTransform().Translate(direction);
    gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * speed * Time::GetDeltaTime());
    //Console::Log(std::to_string(dirY));
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
         Destroy();
    }
}