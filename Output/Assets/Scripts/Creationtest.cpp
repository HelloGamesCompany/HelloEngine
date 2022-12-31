#include "Creationtest.h"
#include "API/API.h"

void Creationtest::Start()
{

}
void Creationtest::Update()
{
	Console::Log("String name 2: " + stringTest);

	if (Input::GetKey(KeyCode::KEY_T) == KeyState::KEY_DOWN)
	{
		Console::Log("Key T pressed");
	}

	if (Input::GetMouseButton(MouseButton::LEFT) == KeyState::KEY_DOWN)
	{
		Console::Log("Left mouse button pressed");
	}

	std::string goName = testGameObject.GetName();
	Console::Log("Game object name: " + goName);
}