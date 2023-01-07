#include "Creationtest.h"

HELLO_ENGINE_API_C Creationtest* CreateCreationtest(ScriptToInspectorInterface* script)
{
	Creationtest* classInstance = new Creationtest();

	script->AddDragFloat("creation test", &classInstance->creationTestFloat);
	script->AddDragInt("int test 2", &classInstance->intTest);
	script->AddDragBoxGameObject("test game object", &classInstance->testGameObject);

	return classInstance;
}

void Creationtest::Start()
{

}
void Creationtest::Update()
{
	std::string goName = testGameObject.GetName();	
	Console::Log("Game object name: " + goName);
}