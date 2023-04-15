#include "ChangeSceneButton.h"
HELLO_ENGINE_API_C ChangeSceneButton* CreateChangeSceneButton(ScriptToInspectorInterface* script)
{
	ChangeSceneButton* classInstance = new ChangeSceneButton();
	script->AddDragBoxUIButton("Button", &classInstance->button);
	script->AddInputBox("Scene", &classInstance->sceneName);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ChangeSceneButton::Start()
{

}
void ChangeSceneButton::Update()
{
	if (button.OnPress())
	{
		Scene::LoadScene(sceneName.c_str());
	}
}