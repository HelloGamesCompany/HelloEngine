#include "TestAPI.h"
HELLO_ENGINE_API_C TestAPI* CreateTestAPI(ScriptToInspectorInterface* script)
{
	TestAPI* classInstance = new TestAPI();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void TestAPI::Start()
{
	/*Game::FindGameObjectsWithTag("1", &gameObjects[0], 3);
	for (int i = 0; i < 10; ++i)
	{
		gameObjects[i].SetActive(false);
	}*/
	gameObject.GetChildren(&gameObjects[0]);
	for (int i = 0; i < 10; ++i)
	{
		gameObjects[i].SetActive(false);
	}
}

void TestAPI::Update()
{

}