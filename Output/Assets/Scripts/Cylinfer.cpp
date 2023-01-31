#include "Cylinfer.h"
HELLO_ENGINE_API_C Cylinfer* CreateCylinfer(ScriptToInspectorInterface* script)
{
	Cylinfer* classInstance = new Cylinfer();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Cylinfer::Start()
{
	Console::Log("Cyinfer");
}
void Cylinfer::Update()
{

}