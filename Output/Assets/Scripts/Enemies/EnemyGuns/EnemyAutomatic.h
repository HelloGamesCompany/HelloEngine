#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
//#include "../EnemyGun.h"
class EnemyAutomatic :HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	//void Shoot()override;
	
	float fullShotCooldown;
	float shotCooldown;
	bool canShoot = true;


};

