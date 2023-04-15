#include "ShotgunBombExplosion.h"
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

}

void ShotgunBombExplosion::OnCollisionEnter(API::API_RigidBody other)
{
    if (!triggerActive) return;

    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Enemy")
    {
        // do damage
    }

    triggerActive = false;
    shotgunBomb.SetActive(false);
}