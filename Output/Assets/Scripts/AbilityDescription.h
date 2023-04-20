#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class AbilityDescription : HelloBehavior
{
public:


    API_GameObject currentDescription;
    API_GameObject nextDescription;
    API_GameObject previousDescription;
    API_UIButton HoveredButton;


	void Start() override; 
	void Update() override;
};

