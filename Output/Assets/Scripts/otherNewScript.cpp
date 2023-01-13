#include "otherNewScript.h"
HELLO_ENGINE_API_C otherNewScript* CreateotherNewScript(ScriptToInspectorInterface* script)
{
	otherNewScript* classInstance = new otherNewScript();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void otherNewScript::Start()
{

}
void otherNewScript::Update()
{

}