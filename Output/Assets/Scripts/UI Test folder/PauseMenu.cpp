#include "PauseMenu.h"
HELLO_ENGINE_API_C PauseMenu* CreatePauseMenu(ScriptToInspectorInterface* script)
{
	PauseMenu* classInstance = new PauseMenu();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	//script->AddDragBoxUIImage("Map", &classInstance->mapImage);
	return classInstance;
}

void PauseMenu::Start()
{

}
void PauseMenu::Update()
{

}