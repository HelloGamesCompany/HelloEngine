#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class UIManager;

class LevelEndTrigger : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void OnCollisionEnter(API_RigidBody other);

	API_GameObject UIManagerGO;
	UIManager* manager;
};

