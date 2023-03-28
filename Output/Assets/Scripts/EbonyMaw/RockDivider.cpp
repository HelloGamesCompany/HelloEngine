#include "RockDivider.h"
#include "../Player/PlayerStats.h"
#include "../EbonyMaw/BossLoop.h"
#include "../EbonyMaw/BossAttacks.h"


HELLO_ENGINE_API_C RockDivider* CreateRockDivider(ScriptToInspectorInterface* script)
{
	RockDivider* classInstance = new RockDivider();
	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	script->AddDragBoxGameObject("stone1", &classInstance->stone1);
	script->AddDragBoxGameObject("stone2", &classInstance->stone2);
	script->AddDragBoxGameObject("stone3", &classInstance->stone3);
	script->AddDragBoxGameObject("stone4", &classInstance->stone4);
	script->AddDragBoxGameObject("stone5", &classInstance->stone5);
	script->AddDragBoxGameObject("stone6", &classInstance->stone6);
	script->AddDragBoxGameObject("stone7", &classInstance->stone7);
	script->AddDragBoxGameObject("stone8", &classInstance->stone8);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void RockDivider::Start()
{

}
void RockDivider::Update()
{

	API_GameObject go = boss;
	BossAttacks* bAttacks = (BossAttacks*)go.GetScript("BossAttacks");
	if (rockDivided == true) {
		dt = Time::GetDeltaTime();
		stoneTime += dt;
		gameObject.GetTransform().SetRotation(0, 0, 0);
		stone1.GetTransform().SetPosition(stone1.GetTransform().GetLocalPosition().x, stone1.GetTransform().GetLocalPosition().y, stone1.GetTransform().GetLocalPosition().z + 0.1);
		stone2.GetTransform().SetPosition(stone2.GetTransform().GetLocalPosition().x + 0.1, stone2.GetTransform().GetLocalPosition().y, stone2.GetTransform().GetLocalPosition().z + 0.1);
		stone3.GetTransform().SetPosition(stone3.GetTransform().GetLocalPosition().x + 0.1, stone3.GetTransform().GetLocalPosition().y, stone3.GetTransform().GetLocalPosition().z);
		stone4.GetTransform().SetPosition(stone4.GetTransform().GetLocalPosition().x + 0.1, stone4.GetTransform().GetLocalPosition().y, stone4.GetTransform().GetLocalPosition().z - 0.1);
		stone5.GetTransform().SetPosition(stone5.GetTransform().GetLocalPosition().x, stone5.GetTransform().GetLocalPosition().y, stone5.GetTransform().GetLocalPosition().z - 0.1);
		stone6.GetTransform().SetPosition(stone6.GetTransform().GetLocalPosition().x - 0.1, stone6.GetTransform().GetLocalPosition().y, stone6.GetTransform().GetLocalPosition().z - 0.1);
		stone7.GetTransform().SetPosition(stone7.GetTransform().GetLocalPosition().x - 0.1, stone7.GetTransform().GetLocalPosition().y, stone7.GetTransform().GetLocalPosition().z);
		stone8.GetTransform().SetPosition(stone8.GetTransform().GetLocalPosition().x - 0.1, stone8.GetTransform().GetLocalPosition().y, stone8.GetTransform().GetLocalPosition().z + 0.1);
	
		if (bAttacks->stoneLifeTime < stoneTime) {
			bAttacks->ReturnRock(&gameObject);
			
			stone1.GetTransform().SetPosition(0,0,0);
			stone2.GetTransform().SetPosition(0,0,0);
			stone3.GetTransform().SetPosition(0,0,0);
			stone4.GetTransform().SetPosition(0,0,0);
			stone5.GetTransform().SetPosition(0,0,0);
			stone6.GetTransform().SetPosition(0,0,0);
			stone7.GetTransform().SetPosition(0,0,0);
			stone8.GetTransform().SetPosition(0,0,0);
			rockDivided = false;
		}
	}
}

void RockDivider::OnCollisionEnter(API::API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	API_GameObject go = boss;
	BossAttacks* bAttacks = (BossAttacks*)go.GetScript("BossAttacks");

	if (detectionName == "Player") {
		PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
		pStats->TakeDamage(bAttacks->rockDmg);
		bAttacks->ReturnRock(&gameObject);
	}
	else if (detectionName != "Projectile" && detectionName != "Boss" && bAttacks->throwing == true) {
		rockDivided = true;
	}
}
