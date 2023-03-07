#include "Projectile.h"
HELLO_ENGINE_API_C Projectile* CreateProjectile(ScriptToInspectorInterface* script)
{
    Projectile* classInstance = new Projectile();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Lifetime", &classInstance->lifeTime);
    script->AddDragFloat("Damage", &classInstance->damage);
    script->AddDragFloat("Resistance Damage", &classInstance->resistanceDamage);
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

    gameObject.GetTransform().Translate(direction);
}

void Projectile::Destroy()
{
    gameObject.Destroy();
}