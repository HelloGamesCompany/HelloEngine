#include "feedbackMission.h"
#include "..\..\Quests\Secondary_Quests\Mision_LikeThe80s.h"
#include "..\..\Quests\Secondary_Quests\Mision_Masacre.h"
#include "..\..\Quests\Secondary_Quests\Mision_SomethingPersonal.h"
#include "..\..\Quests\Secondary_Quests\Mision_TheRuleOfFive.h"
#include "..\..\Player\PlayerStorage.h"

HELLO_ENGINE_API_C feedbackMission* CreatefeedbackMission(ScriptToInspectorInterface* script)
{
	feedbackMission* classInstance = new feedbackMission();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxGameObject("Player", &classInstance->player);
	script->AddDragBoxGameObject("Mission 1", &classInstance->missionPostit1);
	script->AddDragBoxGameObject("Mission 2", &classInstance->missionPostit2);
	script->AddDragBoxGameObject("Missions Script", &classInstance->missionsGO);

	script->AddDragBoxTextureResource("Material mission 1 Finished", &classInstance->mission1Finish);
	script->AddDragBoxTextureResource("Material mission 2 Finished", &classInstance->mission2Finish);

	
	return classInstance;
}

void feedbackMission::Start()
{
	playerStorage = (PlayerStorage*)player.GetScript("PlayerStorage");
	mision_LikeThe = (Mision_LikeThe80s*)missionsGO.GetScript("Mision_LikeThe80s");
	mision_Masacre = (Mision_Masacre*)missionsGO.GetScript("Mision_Masacre");
	//mision_Personal = (Mision_SomethingPersonal*)missionsGO.GetScript("Mision_SomethingPersonal");
}

void feedbackMission::Update()
{
	switch (playerStorage->levelIndex)
	{
    case 0: // hub
       // no casettes in hub
        break;
    case 1: // level 1
		if (mision_LikeThe->misionCompleted)
		{
			missionPostit1.GetMaterialCompoennt().ChangeAlbedoTexture(mission1Finish);
		}
		//Second mission in revision
        break;
    case 2: // level 2
		if (mision_Masacre->misionCompleted)
		{
			missionPostit1.GetMaterialCompoennt().ChangeAlbedoTexture(mission1Finish);
		}
		/*if (mision_Personal->misionCompleted)
		{
			missionPostit2.GetMaterialCompoennt().ChangeAlbedoTexture(mission2Finish);
		}*/
        break;
    case 3: // level 3
        break;
    case 4: // level 4
        break;
    case 5: // level 5
        break;
    default:
        Console::Log("Wrong level index", API::Console::MessageType::ERR);
        break;
	}
}