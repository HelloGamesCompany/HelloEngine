#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStorage;
class EnemyDieEvent;

class Mision_TheRuleOfFive : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_GameObject playerStorageGO;
	PlayerStorage* playerStorage;
	EnemyDieEvent* enemyDieEventRef;

	//int enemiessize = 0;

	//std::vector <API_GameObject> enemies;

	float ruleOfFiveTimer = 0.0f;

	bool misionCompleted = false;

};

