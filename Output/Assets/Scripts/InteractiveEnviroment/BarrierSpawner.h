#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class EnemyMeleeSpawner;

class BarrierSpawner : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;


	API_GameObject spawner1;
	API_GameObject spawner2;
	API_GameObject Lamp1;
	API_GameObject Lamp2;

	uint materialLamps;
	uint materialLamp1;
	uint materialLamp2;

	EnemyMeleeSpawner* enemyMeleeSpawner1Ref;
	EnemyMeleeSpawner* enemyMeleeSpawner2Ref;

	bool Lamp1TextChanged = false;
	bool Lamp2TextChanged = false;
};

