#include "Stone.h"
#include "../Player/PlayerStats.h"
#include "../EbonyMaw/BossAttacks.h"

HELLO_ENGINE_API_C Stone* CreateStone(ScriptToInspectorInterface* script)
{
	Stone* classInstance = new Stone();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Stone::Start()
{

}
void Stone::Update()
{

}

void Stone::OnCollisionEnter(API::API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	API_GameObject go = boss;
	BossAttacks* bAttacks = (BossAttacks*)go.GetScript("BossAttacks");

	if (detectionName == "Player") {
		PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
		pStats->TakeDamage(10);
	}
}
