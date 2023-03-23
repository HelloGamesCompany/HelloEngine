#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class HpBar : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	float hp = 100;
	float maxHp = 100;

	float FirstPosX;
	float FirstPosY;
	float FirstPosZ;

};

