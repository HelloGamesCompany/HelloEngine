#include "compilingTest2.h"
HELLO_ENGINE_API_C compilingTest2* CreatecompilingTest2(ScriptToInspectorInterface* script)
{
	compilingTest2* classInstance = new compilingTest2();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void compilingTest2::Start()
{

}
void compilingTest2::Update()
{

}