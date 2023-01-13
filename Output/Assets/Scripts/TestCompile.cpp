#include "TestCompile.h"
HELLO_ENGINE_API_C TestCompile* CreateTestCompile(ScriptToInspectorInterface* script)
{
	TestCompile* classInstance = new TestCompile();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void TestCompile::Start()
{

}
void TestCompile::Update()
{

}