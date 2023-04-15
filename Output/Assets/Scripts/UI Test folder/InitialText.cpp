#include "InitialText.h"
HELLO_ENGINE_API_C InitialText* CreateInitialText(ScriptToInspectorInterface* script)
{
	InitialText* classInstance = new InitialText();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIButton("Continue", &classInstance->button);
	return classInstance;
}

void InitialText::Start()
{

}
void InitialText::Update()
{
	if (button.OnPress())
	{
		gameObject.SetActive(false);
	}
}