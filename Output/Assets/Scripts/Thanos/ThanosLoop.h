#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "../Player/PlayerStats.h"

#include "API/API.h"

class ThanosLoop : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void OnCollisionEnter(API::API_RigidBody other);

	void TakeDamage(float damage);

	float hp;

	float meleeDmg = 20.0f;

};

