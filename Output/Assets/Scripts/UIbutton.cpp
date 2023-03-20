#include "UIbutton.h"
HELLO_ENGINE_API_C UIbutton* CreateUIbutton(ScriptToInspectorInterface* script)
{
	UIbutton* classInstance = new UIbutton();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIButton("button", &classInstance->prova);
	return classInstance;
}

void UIbutton::Start()
{

}
void UIbutton::Update()
{
	if (prova.OnPress())
	{
		Console::Log("im pressed");
	}
}