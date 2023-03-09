#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class AnimationTest : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_AnimationPlayer anim;
	uint animWalk = 0;
};

