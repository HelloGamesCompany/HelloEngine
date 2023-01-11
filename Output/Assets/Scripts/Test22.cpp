#include "Test22.h"
HELLO_ENGINE_API_C Test22* CreateTest22(ScriptToInspectorInterface* script)
{
	Test22* classInstance = new Test22();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Test22::Start()
{

}
void Test22::Update()
{

}