#include "Mision_TheRuleOfFive.h"
HELLO_ENGINE_API_C Mision_TheRuleOfFive* CreateMision_TheRuleOfFive(ScriptToInspectorInterface* script)
{
	Mision_TheRuleOfFive* classInstance = new Mision_TheRuleOfFive();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragInt("NumberOfEnemiesInLevel", &classInstance->enemiessize);
	script->AddDragBoxGameObject("Player Storage", &classInstance->playerStorageGO);

	return classInstance;
}

void Mision_TheRuleOfFive::Start()
{

	playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
	if (playerStorage == nullptr) Console::Log("PlayerStorage missing in GetDiviner Script.");

	Game::FindGameObjectsWithTag("Enemy", &enemies[0], enemiessize);
}
void Mision_TheRuleOfFive::Update()
{

	if (ruleOfFiveTimer < 5.0f)
	{

		ruleOfFiveTimer += Time::GetDeltaTime();


	}

}