#include "FireDamage.h"
#include "BossAttacks.h"

HELLO_ENGINE_API_C FireDamage* CreateFireDamage(ScriptToInspectorInterface* script)
{
	FireDamage* classInstance = new FireDamage();
	script->AddDragBoxGameObject("BOSS", &classInstance->boss);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void FireDamage::Start()
{
	bAttacks = (BossAttacks*)boss.GetScript("BossAttacks");
}
void FireDamage::Update()
{

}


void FireDamage::OnCollisionEnter(API::API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();

	if (detectionName == "Player") {
		PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
		pStats->TakeDamage(bAttacks->fireDmg, 0);
	}

}
