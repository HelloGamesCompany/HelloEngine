#include "EnemyMeleeSpawner.h"
#include "../Enemies/EnemyMeleeMovement.h"
#include "../Enemies/Enemy.h"

HELLO_ENGINE_API_C EnemyMeleeSpawner* CreateEnemyMeleeSpawner(ScriptToInspectorInterface* script)
{
	EnemyMeleeSpawner* classInstance = new EnemyMeleeSpawner();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragInt("Enemy Pool Size", &classInstance->spawnPoolSize);
	script->AddDragBoxParticleSystem("Particles", &classInstance->enemySpawnParticle);
	script->AddDragBoxGameObject("Target", &classInstance->target);
	script->AddDragBoxGameObject("Action zone", &classInstance->actionZone);
	script->AddDragBoxRigidBody("Action Rb zone", &classInstance->zoneRb);
	script->AddDragBoxGameObject("Point 1", &classInstance->listPoints[0]);
	script->AddDragBoxGameObject("Point 2", &classInstance->listPoints[1]);
	script->AddDragBoxGameObject("Point 3", &classInstance->listPoints[2]);

	script->AddDragBoxAnimationPlayer("Animation Player", &classInstance->animationPlayer);

	return classInstance;
}

void EnemyMeleeSpawner::Start()
{

	enemiesInSpawn.resize(spawnPoolSize);


	for (size_t i = 0; i < spawnPoolSize; i++)
	{

		enemiesInSpawn[i] = Game::CreateGameObject("Enemy", "Enemy");
		enemiesInSpawn[i].AddScript("Enemy");
		enemiesInSpawn[i].AddScript("EnemyMeleeMovement");
		enemiesInSpawn[i].AddParticleSystem(enemySpawnParticle);
		Enemy* enemyMelee = (Enemy*)enemiesInSpawn[i].GetScript("Enemy");
		enemyMelee->enemyRb = enemiesInSpawn[i].CreateRigidBodyBox(API_Vector3(0.0f, 1.2f, -0.5f), API_Vector3(0.0f, 0.0f, 0.0f), API_Vector3(2.3f, 2.0f, 2.9f), false);
		EnemyMeleeMovement* enemyMeleeMov = (EnemyMeleeMovement*)enemiesInSpawn[i].GetScript("EnemyMeleeMovement");
		enemyMeleeMov->target = this->target;
		enemyMeleeMov->actionZone = this->actionZone;
		enemyMeleeMov->zoneRb = this->zoneRb;
		enemyMeleeMov->listPoints[0] = listPoints[0];
		enemyMeleeMov->listPoints[1] = listPoints[1];
		enemyMeleeMov->listPoints[2] = listPoints[2];

		//API_GameObject enemy_melee = Game::CreateGameObject("enemy_melee1", "enemy_melee1", &enemiesInSpawn[i]);
		////enemy_melee.AddSkinnedmeshrenderer
		//enemy_melee = 

	}
}
void EnemyMeleeSpawner::Update()
{

}

void EnemyMeleeSpawner::OnCollisionEnter(API_RigidBody other)
{
}
