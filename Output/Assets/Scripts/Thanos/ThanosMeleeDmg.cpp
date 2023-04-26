#include "ThanosMeleeDmg.h"
#include "../Player/PlayerStats.h"
HELLO_ENGINE_API_C ThanosMeleeDmg* CreateThanosMeleeDmg(ScriptToInspectorInterface* script)
{
	ThanosMeleeDmg* classInstance = new ThanosMeleeDmg();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ThanosMeleeDmg::Start()
{

}
void ThanosMeleeDmg::Update()
{

}

void ThanosMeleeDmg::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionName == "Player")
    {
        PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
        pStats->TakeDamage(meleeDmg, 0);
    }
}