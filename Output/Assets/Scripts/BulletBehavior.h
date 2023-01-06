#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"


class BulletBehavior : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void Die();

	float lifeTime = 5.0f;
};

HELLO_ENGINE_API_C BulletBehavior* CreateBulletBehavior(ScriptToInspectorInterface* script)
{
	BulletBehavior* classInstance = new BulletBehavior();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("Lifetime", &classInstance->lifeTime);
	return classInstance;
}