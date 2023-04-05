#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include <vector>

#include "API/API.h"

class Mision_Masacre : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	std::vector <API_GameObject> enemies;

	int enemiessize = 0;
	int numOfDeadEnemies = 0;

	bool misionCompleted = false;
};

