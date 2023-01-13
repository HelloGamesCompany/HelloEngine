#include "NewTestCompile.h"
HELLO_ENGINE_API_C NewTestCompile* CreateNewTestCompile(ScriptToInspectorInterface* script)
{
	NewTestCompile* classInstance = new NewTestCompile();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void NewTestCompile::Start()
{

}
void NewTestCompile::Update()
{

}