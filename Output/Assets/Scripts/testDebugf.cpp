#include "testDebugf.h"
HELLO_ENGINE_API_C testDebugf* CreatetestDebugf(ScriptToInspectorInterface* script)
{
	testDebugf* classInstance = new testDebugf();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void testDebugf::Start()
{

}
void testDebugf::Update()
{

}