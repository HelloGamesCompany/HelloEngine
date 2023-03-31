#include "ExplosionWave.h"
HELLO_ENGINE_API_C ExplosionWave* CreateExplosionWave(ScriptToInspectorInterface* script)
{
	ExplosionWave* classInstance = new ExplosionWave();
	script->AddDragFloat("ExplosionWaveDmg", &classInstance->explosionDmg);
	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ExplosionWave::Start()
{
	bAttacks = (BossAttacks*)boss.GetScript("BossAttacks");
}
void ExplosionWave::Update()
{
}

void ExplosionWave::OnCollisionEnter(API::API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();

	if (detectionName == "Player" && gameObject.GetTransform().GetLocalScale().x < 1.25f) {
		PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
		pStats->TakeDamage(explosionDmg);
	}
	else if (detectionName == "Player") {

		PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
		pStats->TakeDamage(1);
	}
}
