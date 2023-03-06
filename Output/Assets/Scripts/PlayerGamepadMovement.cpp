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
	_angle = 0.0f;
}
void PlayerGamepadMovement::Update()
{
	//RIGHT AXIS
	GamepadAim();

	//LEFT AXIS
	
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



	//AIM TO TOP
	if (Input::GetGamePadButton(GamePadButton::BUTTON_UP) == KeyState::KEY_REPEAT /*||Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTY) < -10000*/)
	{
		gameObject.GetTransform().SetRotation(0, 0, 0);

	}
	//AIM TO RIGHT
	if (Input::GetGamePadButton(GamePadButton::BUTTON_RIGHT) == KeyState::KEY_REPEAT /*|| Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTX) > 10000*/)
	{
		gameObject.GetTransform().SetRotation(0, 270, 0);

	}
	//AIM TO DOWN
	if (Input::GetGamePadButton(GamePadButton::BUTTON_DOWN) == KeyState::KEY_REPEAT /*|| Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTY) > 10000*/ )
	{
		gameObject.GetTransform().SetRotation(0, 180, 0);

	}
	//AIM TO LEFT
	if (Input::GetGamePadButton(GamePadButton::BUTTON_LEFT) == KeyState::KEY_REPEAT/* || Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTX) < -10000*/)
	{
		gameObject.GetTransform().SetRotation(0, 90, 0);

	}

	if (Input::GetGamePadButton(GamePadButton::BUTTON_A) == KeyState::KEY_DOWN)
	{
		gameObject.GetTransform().SetPosition({ 0,0,0 });
	}

	if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN)
	{
		int a = gameObject.GetTransform().GetGlobalRotation().y;
		
		Console::Log(std::to_string(a));
	}
}

void PlayerGamepadMovement::GamepadAim()
{

	API_Vector2 mousePos;
	
	mousePos.x = Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTX);
	mousePos.y = -Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTY);


	API_Vector2 playerPos ;
	playerPos.x = gameObject.GetTransform().GetGlobalPosition().x;
	playerPos.y = gameObject.GetTransform().GetGlobalPosition().y;
	API_Vector2 lookDir;
	
	lookDir.x = (mousePos.x );
	lookDir.y = (mousePos.y );
	API_Vector2 normLookDir ;
	normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));

	if (Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTX) > 10000 || Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTX) < -10000
		|| Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTY) > 10000 || Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTY) < -10000)
	{
	 _angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
	}
	

	if (Input::GetKey(KeyCode::KEY_F) == KeyState::KEY_DOWN)
	{
		Console::Log(std::to_string(_angle));

		Console::Log(std::to_string(Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTX)));
		Console::Log(std::to_string(Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTY)));

		Console::Log(std::to_string(mousePos.x));
		Console::Log(std::to_string(mousePos.y));


	}
	gameObject.GetTransform().SetRotation(0, _angle, 0);

}
