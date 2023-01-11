#include "Test4.h"
HELLO_ENGINE_API_C Test4* CreateTest4(ScriptToInspectorInterface* script)
{
	Test4* classInstance = new Test4();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Test4::Start()
{

}
void Test4::Update()
{

}