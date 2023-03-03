#include "PlayerGamepadMovement.h"
HELLO_ENGINE_API_C PlayerGamepadMovement* CreatePlayerGamepadMovement(ScriptToInspectorInterface* script)
{
	PlayerGamepadMovement* classInstance = new PlayerGamepadMovement();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragFloat("Velocity", &classInstance->vel);


	return classInstance;
}

void PlayerGamepadMovement::Start()
{

}
void PlayerGamepadMovement::Update()
{
	//LEFT AXIS

	/*if (gameObject.GetTransform().GetLocalRotation().y > 360)
	{
		gameObject.GetTransform().SetRotation(0,0,0);
	}
	else if (gameObject.GetTransform().GetLocalRotation().y < -0)
	{
		gameObject.GetTransform().SetRotation(0, 360, 0);
	}*/

	if (Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTX) > 10000)
	{
		gameObject.GetTransform().Translate(-0.05f, 0, 0);
	}
	if (Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTX) < -10000)
	{
		gameObject.GetTransform().Translate(0.05f, 0, 0);
	}

	if (Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY) > 10000)
	{
		gameObject.GetTransform().Translate(0, 0, -0.05f);
	}
	if (Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY) < -10000)
	{
		gameObject.GetTransform().Translate(0, 0, 0.05f);
	}

	//RIGHT AXIS

	if ((Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTX) > 10000) && (gameObject.GetTransform().GetLocalRotation().y >= 270 /*(API_Vector3)(0, 270, 0 )*/))
	{
		gameObject.GetTransform().Rotate(0,-1,0);
	}
	if ((Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTX) < -10000) && (gameObject.GetTransform().GetLocalRotation().y >= 90 ))
	{
		gameObject.GetTransform().Rotate(0, 1, 0);
	}

	if (Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTY) > 10000 && (gameObject.GetTransform().GetLocalRotation().y >= (360||0)))
	{
		gameObject.GetTransform().Rotate(0, -1, 0);
	}
	if (Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTY) < -10000 && (gameObject.GetTransform().GetLocalRotation().y >= 180))
	{
		gameObject.GetTransform().Rotate(0, 1, 0);
	}

	if (Input::GetGamePadButton(GamePadButton::BUTTON_A) == KeyState::KEY_DOWN)
	{
		gameObject.GetTransform().SetPosition({ 0,0,0 });
	}

	if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN)
	{
		int a = gameObject.GetTransform().GetLocalRotation().y;
		
		Console::Log(std::to_string(a));
	}
}