#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class EnemyDrop : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	int dropIndex;
	// 0 -> laser ammo
	// 1 -> first aid kit
	// 2 -> speed power up
	// 3 -> firerate power up
	// 4 -> shield power up
	// 5 -> max ammo power up
	// 6 -> slow time power up
};

