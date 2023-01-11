#include "Test3.h"
HELLO_ENGINE_API_C Test3* CreateTest3(ScriptToInspectorInterface* script)
{
	Test3* classInstance = new Test3();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Test3::Start()
{

}
void Test3::Update()
{

}