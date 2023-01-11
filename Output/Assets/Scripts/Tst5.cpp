#include "Tst5.h"
HELLO_ENGINE_API_C Tst5* CreateTst5(ScriptToInspectorInterface* script)
{
	Tst5* classInstance = new Tst5();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Tst5::Start()
{

}
void Tst5::Update()
{

}