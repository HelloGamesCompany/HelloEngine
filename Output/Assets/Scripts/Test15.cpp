#include "Test15.h"
HELLO_ENGINE_API_C Test15* CreateTest15(ScriptToInspectorInterface* script)
{
	Test15* classInstance = new Test15();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Test15::Start()
{

}
void Test15::Update()
{

}