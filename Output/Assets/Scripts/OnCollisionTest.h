#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class OnCollisionTest : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void OnCollisionEnter(API::API_RigidBody other) override;
	void OnCollisionStay(API::API_RigidBody other) override;
	void OnCollisionExit(API::API_RigidBody other) override;
};

