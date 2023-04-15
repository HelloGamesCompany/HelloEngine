#include "HUBTextManager.h"
HELLO_ENGINE_API_C HUBTextManager* CreateHUBTextManager(ScriptToInspectorInterface* script)
{
	HUBTextManager* classInstance = new HUBTextManager();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Panel text 1", &classInstance->fromLevel1Panel);
	script->AddDragBoxGameObject("Panel text 2", &classInstance->fromLevel2Panel);
	return classInstance;
}

void HUBTextManager::Start()
{
	if (API_QuickSave::GetBool("level2_completed", false))
	{
		fromLevel2Panel.SetActive(true);
	}
	else if (API_QuickSave::GetBool("level1_completed", false))
	{
		fromLevel1Panel.SetActive(true);
	}
}
void HUBTextManager::Update()
{

}