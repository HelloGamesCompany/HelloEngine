#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class EnemyDieEvent 
{
public:
	
	static void enemyDead();

	static int numOfEnemiesDead;

};

