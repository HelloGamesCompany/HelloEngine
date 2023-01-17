#include "deletetest.h"
HELLO_ENGINE_API_C deletetest* Createdeletetest(ScriptToInspectorInterface* script)
{
	deletetest* classInstance = new deletetest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void deletetest::Start()
{

}
void deletetest::Update()
{

}