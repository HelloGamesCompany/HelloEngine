#include "DamageTraker.h"
#include "../Shooting/Projectile.h"
HELLO_ENGINE_API_C DamageTraker* CreateDamageTraker(ScriptToInspectorInterface* script)
{
    DamageTraker* classInstance = new DamageTraker();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void DamageTraker::Start()
{

}

void DamageTraker::Update()
{

}

void DamageTraker::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Projectile")
    {
        Projectile* projectile = (Projectile*)other.GetGameObject().GetScript("Projectile");
        Console::Log(std::to_string(projectile->damage));
    }
}