#include "test534654.h"
HELLO_ENGINE_API_C test534654* Createtest534654(ScriptToInspectorInterface* script)
{
	test534654* classInstance = new test534654();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void test534654::Start()
{

}
void test534654::Update()
{

}