#include "UI_Test.h"
HELLO_ENGINE_API_C UI_Test* CreateUI_Test(ScriptToInspectorInterface* script)
{
	UI_Test* classInstance = new UI_Test();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void UI_Test::Start()
{

}
void UI_Test::Update()
{

}