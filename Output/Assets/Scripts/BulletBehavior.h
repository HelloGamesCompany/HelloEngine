#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API.h"

class BulletBehavior : public HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void Die();

	float lifeTime = 5.0f;
	API_Vector3 direction;
};

