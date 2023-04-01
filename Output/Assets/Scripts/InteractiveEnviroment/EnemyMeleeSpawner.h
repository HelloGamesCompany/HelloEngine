#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class EnemyMeleeSpawner : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void OnCollisionEnter(API_RigidBody other);

	API_GameObject enemy;
	API_GameObject target;
	API_GameObject actionZone;

	API_RigidBody zoneRb;

	API_AnimationPlayer animationPlayer;

	API_GameObject listPoints[5];

	int spawnPoolSize;
	std::vector<API_GameObject> enemiesInSpawn;
	API_ParticleSystem enemySpawnParticle;
};

