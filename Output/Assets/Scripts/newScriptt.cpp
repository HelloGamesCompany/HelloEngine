#include "newScriptt.h"
HELLO_ENGINE_API_C newScriptt* CreatenewScriptt(ScriptToInspectorInterface* script)
{
	newScriptt* classInstance = new newScriptt();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void newScriptt::Start()
{

}
void newScriptt::Update()
{

}