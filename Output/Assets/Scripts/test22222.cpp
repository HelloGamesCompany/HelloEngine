#include "test22222.h"
HELLO_ENGINE_API_C test22222* Createtest22222(ScriptToInspectorInterface* script)
{
	test22222* classInstance = new test22222();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void test22222::Start()
{

}
void test22222::Update()
{

}