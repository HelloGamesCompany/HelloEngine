#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStorage;
class Mision_LikeThe80s;
class Mision_Masacre;
class Mision_SomethingPersonal;

class feedbackMission : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_GameObject missionPostit1;
	API_GameObject missionPostit2;
	API_GameObject missionsGO;
	API_GameObject player;

	uint mission1Finish;
	uint mission2Finish;

	PlayerStorage* playerStorage = nullptr;

	Mision_LikeThe80s* mision_LikeThe = nullptr;
	Mision_Masacre* mision_Masacre = nullptr;
	Mision_SomethingPersonal* mision_Personal = nullptr;
};

