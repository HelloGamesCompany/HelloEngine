#include "CompilingTest3.h"
HELLO_ENGINE_API_C CompilingTest3* CreateCompilingTest3(ScriptToInspectorInterface* script)
{
	CompilingTest3* classInstance = new CompilingTest3();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void CompilingTest3::Start()
{

}
void CompilingTest3::Update()
{

}