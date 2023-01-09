#include "testtt.h"
HELLO_ENGINE_API_C testtt* Createtesttt(ScriptToInspectorInterface* script)
{
	testtt* classInstance = new testtt();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void testtt::Start()
{

}
void testtt::Update()
{

}