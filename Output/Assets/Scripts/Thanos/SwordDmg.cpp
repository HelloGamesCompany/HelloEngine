#include "SwordDmg.h"
#include "../Player/PlayerStats.h"
HELLO_ENGINE_API_C SwordDmg* CreateSwordDmg(ScriptToInspectorInterface* script)
{
	SwordDmg* classInstance = new SwordDmg();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void SwordDmg::Start()
{

}
void SwordDmg::Update()
{

}

void SwordDmg::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionName == "Player")
    {
        PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
        pStats->TakeDamage(swordDmg, 0);
    }
}