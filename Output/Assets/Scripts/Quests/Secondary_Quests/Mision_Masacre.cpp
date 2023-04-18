#include "Mision_Masacre.h"
HELLO_ENGINE_API_C Mision_Masacre* CreateMision_Masacre(ScriptToInspectorInterface* script)
{
	Mision_Masacre* classInstance = new Mision_Masacre();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragInt("NumberOfEnemiesInLevel", &classInstance->enemiessize);

	return classInstance;
}

void Mision_Masacre::Start()
{

	enemies.resize(enemiessize);

	Game::FindGameObjectsWithTag("Enemy", &enemies[0], enemiessize); 

}
void Mision_Masacre::Update()
{
	if (misionCompleted == true)
	{
		Console::Log("MISSION COMPLETED CONGRATULATIONS!");
	}
	else 
	{
		for (int i = 0; i < enemiessize; i++)
		{
			if (enemies[i].IsActive())continue;

			if (enemies[i].IsActive() == false)
			{
				numOfDeadEnemies++;
				if (numOfDeadEnemies == enemiessize)
				{
					misionCompleted = true;
				}
			}
		}

		numOfDeadEnemies = 0;
	
	}
}