#include "RockDivider.h"
#include "../Player/PlayerStats.h"
#include "../EbonyMaw/BossLoop.h"
#include "../EbonyMaw/BossAttacks.h"
//Pau Olmos

HELLO_ENGINE_API_C RockDivider* CreateRockDivider(ScriptToInspectorInterface* script)
{
	RockDivider* classInstance = new RockDivider();
	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	script->AddDragBoxMeshResource("Stone Mesh", &classInstance->stoneMesh);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void RockDivider::Start()
{
	API_GameObject go = boss;
	BossAttacks* bAttacks = (BossAttacks*)go.GetScript("BossAttacks");

	for (size_t i = 0; i < 8; i++)
	{
		API_GameObject stone = Game::CreateGameObject("Stone", "Stone");
		stone.AddMeshRenderer().ChangeMesh(stoneMesh);
		stone.AddMaterial();
		stone.CreateRigidBodyBox((0,0,0), (0, 0, 0), (0.3f, 0.3f, 0.3f), false);
		stone.AddScript("Stone");
		stone.SetActive(true);
		stones.push_back(stone);
	}
}
void RockDivider::Update()
{
	API_GameObject go = boss;
	BossAttacks* bAttacks = (BossAttacks*)go.GetScript("BossAttacks");

	if (rockDivided == true) {
		dt = Time::GetDeltaTime();
		stoneTime += dt;
		bAttacks->ReturnRock(&gameObject);
		gameObject.GetTransform().SetRotation(0, 0, 0);
		stones[0].GetTransform().Translate(0, 0, 0.1f * dt * 100);
		stones[1].GetTransform().Translate(0.1f * dt * 100, 0, 0.1f * dt * 100);
		stones[2].GetTransform().Translate(0.1f * dt * 100, 0, 0);
		stones[3].GetTransform().Translate(0.1f * dt * 100, 0, -0.1f * dt * 100);
		stones[4].GetTransform().Translate(0, 0, -0.1f * dt * 100);
		stones[5].GetTransform().Translate(-0.1f * dt * 100, 0, -0.1f * dt * 100);
		stones[6].GetTransform().Translate(-0.1f * dt * 100, 0, 0);
		stones[7].GetTransform().Translate(-0.1f * dt * 100, 0, 0.1f * dt * 100);
		if (bAttacks->stoneLifeTime < stoneTime) {
			stoneTime = 0;
			rockDivided = false;
		}
		
	}
	else {
		//gameObject.SetActive(true);
		for (int i = 0; i < 8; i++) {
			stones[i].GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition());
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
	else if (detectionName != "Projectile" && detectionName != "Boss" && bAttacks->throwing == true && detectionName != "Stone") {
		rockDivided = true;
	}
}
