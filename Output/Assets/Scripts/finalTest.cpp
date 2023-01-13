#include "finalTest.h"
HELLO_ENGINE_API_C finalTest* CreatefinalTest(ScriptToInspectorInterface* script)
{
	finalTest* classInstance = new finalTest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void finalTest::Start()
{

}
void finalTest::Update()
{

}