#include "Level2FinalText.h"
HELLO_ENGINE_API_C Level2FinalText* CreateLevel2FinalText(ScriptToInspectorInterface* script)
{
	Level2FinalText* classInstance = new Level2FinalText();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIButton("Continute button: ", &classInstance->continueButton);
	return classInstance;
}

void Level2FinalText::Start()
{

}
void Level2FinalText::Update()
{
	if (continueButton.OnPress())
	{
		Scene::LoadScene("WinMenu.HScene");
	}
}