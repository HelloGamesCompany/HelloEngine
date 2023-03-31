#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class BossMovement : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void Seek(API_Vector3 target);
	API_GameObject player;

};

