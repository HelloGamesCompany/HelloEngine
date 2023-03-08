#include "PlayerKeyboardMovement.h"
HELLO_ENGINE_API_C PlayerKeyboardMovement* CreatePlayerKeyboardMovement(ScriptToInspectorInterface* script)
{
	PlayerKeyboardMovement* classInstance = new PlayerKeyboardMovement();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragFloat("Velocity", &classInstance->velk);

	return classInstance;
}

void PlayerKeyboardMovement::Start()
{
	_angle = 0.0f;
}
void PlayerKeyboardMovement::Update()
{

	float dt = Time::GetDeltaTime();
	MouseAim();
//
	//MOVEMENT TO FORWARD
	if (Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT)
	{
		//gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward()* vel );
		gameObject.GetTransform().Translate(0,0,1.0f * velk);
	}

	//MOVEMENT TO RIGHT
	if (Input::GetKey(KeyCode::KEY_D) == KeyState::KEY_REPEAT)
	{
		//gameObject.GetTransform().Translate(gameObject.GetTransform().GetLeft() * vel);
		gameObject.GetTransform().Translate(-1.0f * velk, 0, 0 );
	}

	//MOVEMENT TO BACKWARD
	if (Input::GetKey(KeyCode::KEY_S) == KeyState::KEY_REPEAT)
	{
		//gameObject.GetTransform().Translate(gameObject.GetTransform().GetBackward() * vel);
		gameObject.GetTransform().Translate(0, 0, -1.0f * velk);
	}

	//MOVEMENT TO LEFT
	if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_REPEAT)
	{
		//gameObject.GetTransform().Translate(gameObject.GetTransform().GetRight() * vel);
		gameObject.GetTransform().Translate(1.0f * velk, 0, 0 );
	}

	//AIM TO TOP
	if (Input::GetKey(KeyCode::KEY_UP) == KeyState::KEY_REPEAT)
	{
		gameObject.GetTransform().SetRotation(0,0,0);
	}
	//AIM TO RIGHT
	if (Input::GetKey(KeyCode::KEY_RIGHT) == KeyState::KEY_REPEAT)
	{
		gameObject.GetTransform().SetRotation(0,270,0);
	}
	//AIM TO DOWN
	if (Input::GetKey(KeyCode::KEY_DOWN) == KeyState::KEY_REPEAT)
	{
		gameObject.GetTransform().SetRotation(0,180,0);
	}
	//AIM TO LEFT
	if (Input::GetKey(KeyCode::KEY_LEFT) == KeyState::KEY_REPEAT)
	{
		gameObject.GetTransform().SetRotation(0,90,0);
	}
	
}

void PlayerKeyboardMovement::MouseAim()
{
	API_Vector2 mousePos;
	mousePos.x = Input::GetMouseX() - SCREEN_WIDTH/2 ;
	mousePos.y = Input::GetMouseY() - SCREEN_HEIGHT/2 ;
	
	API_Vector2 playerPos /*= (gameObject.GetTransform().GetGlobalPosition().x, gameObject.GetTransform().GetGlobalPosition().y)*/;
	playerPos.x = gameObject.GetTransform().GetGlobalPosition().x;
	playerPos.y = gameObject.GetTransform().GetGlobalPosition().y;
	API_Vector2 lookDir /*= (mousePos.x - playerPos.x, mousePos.y - playerPos.y)*/;
	lookDir.x = (mousePos.x - playerPos.x);
	lookDir.y = (mousePos.y - playerPos.y);
	//lookDir.x = (mousePos.x );
	//lookDir.y = (mousePos.y );
	API_Vector2 normLookDir /*= lookDir / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2))*/;
	normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	

	//float angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
	 _angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG  ;

	if (Input::GetKey(KeyCode::KEY_F) == KeyState::KEY_DOWN)
	{
		Console::Log(std::to_string(_angle));

		Console::Log(std::to_string(Input::GetMouseX()));
		Console::Log(std::to_string(Input::GetMouseY()));

		Console::Log(std::to_string(mousePos.x));
		Console::Log(std::to_string(mousePos.y));

		
	}
	gameObject.GetTransform().SetRotation(0,-_angle, 0);
}


