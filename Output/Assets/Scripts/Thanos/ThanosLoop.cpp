#include "ThanosLoop.h"
#include "ThanosLoop.h"
HELLO_ENGINE_API_C ThanosLoop* CreateThanosLoop(ScriptToInspectorInterface* script)
{
	ThanosLoop* classInstance = new ThanosLoop();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ThanosLoop::Start()
{

}
void ThanosLoop::Update()
{

}

void ThanosLoop::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    std::string detectionTag = other.GetGameObject().GetTag();

    if (hp > 0) {
        if (detectionName == "Player")
        {
            PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
            pStats->TakeDamage(meleeDmg, 0);
        }
        if (detectionTag == "Projectile") {
            //
        }
    }
}
void ThanosLoop::TakeDamage(float damage)
{
    hp -= damage;
}