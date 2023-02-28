#include "GamePadTest.h"
HELLO_ENGINE_API_C GamePadTest* CreateGamePadTest(ScriptToInspectorInterface* script)
{
	GamePadTest* classInstance = new GamePadTest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void GamePadTest::Start()
{

}
void GamePadTest::Update()
{
	if (Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTX) > 10000)
	{
		gameObject.GetTransform().Translate(0.05f, 0, 0);
	}
	if (Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTX) < -10000)
	{
		gameObject.GetTransform().Translate(-0.05f, 0, 0);
	}

	if (Input::GetGamePadButton(GamePadButton::BUTTON_A) == KeyState::KEY_DOWN)
	{
		gameObject.GetTransform().SetPosition({ 0,0,0 });
	}
}