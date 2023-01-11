#include "Creationtest.h"
HELLO_ENGINE_API_C Creationtest* CreateCreationtest(ScriptToInspectorInterface* script)
{
	Creationtest* classInstance = new Creationtest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Creationtest::Start()
{

}
void Creationtest::Update()
{

}