#include "testingggg.h"

HELLO_ENGINE_API_C testingggg* Createtestingggg(ScriptToInspectorInterface* script)
{
	testingggg* classInstance = new testingggg();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void testingggg::Start()
{

}
void testingggg::Update()
{

}