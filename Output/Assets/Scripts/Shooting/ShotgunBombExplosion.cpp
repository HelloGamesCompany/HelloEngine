#include "ShotgunBombExplosion.h"
#include "../Enemies/Enemy.h"
HELLO_ENGINE_API_C ShotgunBombExplosion* CreateShotgunBombExplosion(ScriptToInspectorInterface* script)
{
    ShotgunBombExplosion* classInstance = new ShotgunBombExplosion();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void ShotgunBombExplosion::Start()
{

}

void ShotgunBombExplosion::Update()
{
    if (destroy)
    {
        shotgunBomb.SetActive(false);
        destroy = false;
    }
}

void ShotgunBombExplosion::OnCollisionEnter(API::API_RigidBody other)
{
    if (!triggerActive) return;

    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Enemy")
    {
        Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
        if (enemy) enemy->TakeDamage(damage, resistanceDamage);
    }
    else if (detectionTag == "Boss")
    {
        Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
        if (enemy) enemy->TakeDamage(damage, resistanceDamage);
    }

    triggerActive = false;
    destroy = true;
}