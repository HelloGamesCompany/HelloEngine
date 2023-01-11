#include "Test2.h"
HELLO_ENGINE_API_C Test2* CreateTest2(ScriptToInspectorInterface* script)
{
	Test2* classInstance = new Test2();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Test2::Start()
{

}
void Test2::Update()
{

}