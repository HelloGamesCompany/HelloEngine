#include "EnemyMeleeSpawner.h"
#include "../Enemies/EnemyMeleeMovement.h"
#include "../Enemies/Enemy.h"

HELLO_ENGINE_API_C EnemyMeleeSpawner* CreateEnemyMeleeSpawner(ScriptToInspectorInterface* script)
{
	EnemyMeleeSpawner* classInstance = new EnemyMeleeSpawner();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxPrefabResource("Enemy Prefab", &classInstance->enemyPrefabRes);
	script->AddDragBoxGameObject(" Parent of the Pool", &classInstance->parent);
	script->AddDragInt("Enemy Pool Size", &classInstance->spawnPoolSize);
	script->AddDragBoxParticleSystem("Particles", &classInstance->enemySpawnParticle);
	script->AddDragBoxGameObject("Target", &classInstance->target);
	script->AddDragBoxGameObject("Action zone", &classInstance->actionZone);
	script->AddDragBoxRigidBody("Action Rb zone", &classInstance->zoneRb);
	script->AddDragBoxGameObject("Point 1", &classInstance->listPoints[0]);
	script->AddDragBoxGameObject("Point 2", &classInstance->listPoints[1]);
	script->AddDragBoxGameObject("Point 3", &classInstance->listPoints[2]);

	return classInstance; 
}

void EnemyMeleeSpawner::Start()
{

	enemiesInSpawn.resize(spawnPoolSize);


	for (size_t i = 0; i < spawnPoolSize; i++)
	{
		
		API_GameObject rootBoneRef;
		API_SkinnedMeshRenderer skinnedMeshRef;

		std::string detectionTag;

		enemiesInSpawn[i] = Game::InstancePrefab(enemyPrefabRes, parent);

		enemiesInSpawn[i].GetChildren(childs);

		for(API_GameObject& var : childs) 
		{
			detectionTag = var.GetTag();

			if (detectionTag == "SkinnedMesh")
			{
				skinnedMeshRef = var.GetSkinnedMeshRender();
			}
			else if (detectionTag == "RootBone")
			{  
				rootBoneRef = var;
			}
		}

		skinnedMeshRef.SetRootBone(rootBoneRef);

		EnemyMeleeMovement* enemyMeleeMov = (EnemyMeleeMovement*)enemiesInSpawn[i].GetScript("EnemyMeleeMovement");
		if (enemyMeleeMov != nullptr)
		{

			enemyMeleeMov->target = target;
			enemyMeleeMov->actionZone = actionZone;
			enemyMeleeMov->zoneRb = zoneRb;
			enemyMeleeMov->listPoints[0] = listPoints[0];
			enemyMeleeMov->listPoints[1] = listPoints[1];
			enemyMeleeMov->listPoints[2] = listPoints[2];
		}
		else
		{
		}
		
		//API_GameObject enemy_melee = Game::CreateGameObject("enemy_melee1", "enemy_melee1", &enemiesInSpawn[i]);
		////enemy_melee.AddSkinnedmeshrenderer
		//enemy_melee = 


		enemiesInSpawn[i].GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition());
	}
}
void EnemyMeleeSpawner::Update()
{
	
}

void EnemyMeleeSpawner::OnCollisionEnter(API_RigidBody other)
{
}
