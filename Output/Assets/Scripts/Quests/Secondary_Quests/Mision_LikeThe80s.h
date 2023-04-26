#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStorage;

class Mision_LikeThe80s : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_GameObject playerStorageGO;
	PlayerStorage* playerStorage;

	bool misionCompleted = false;

};

