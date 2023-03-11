#include "PlayerKeyboardMovement.h"
#include "SwapCam.h"
HELLO_ENGINE_API_C PlayerKeyboardMovement* CreatePlayerKeyboardMovement(ScriptToInspectorInterface* script)
{
	PlayerKeyboardMovement* classInstance = new PlayerKeyboardMovement();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragFloat("Velocity", &classInstance->velk);
	script->AddDragFloat("Max Velocity", &classInstance->maxVel);
	script->AddDragFloat("Acceleration", &classInstance->accel);
	script->AddDragFloat("Brake", &classInstance->brake);
	script->AddDragBoxGameObject("Aux Cam", &classInstance->finalCam);
	script->AddDragBoxTransform("Camera player", &classInstance->MainCam);
	script->AddDragBoxTransform("Camera Starship", &classInstance->StarShipCam);
	return classInstance;
}

void PlayerKeyboardMovement::Start()
{
	_angle = 0.0f;
}
void PlayerKeyboardMovement::Update()
{
	float dt = Time::GetDeltaTime();

	SwapCam* cameraScript = (SwapCam*)finalCam.GetScript("SwapCam");

	if (cameraScript != nullptr) {
		if (cameraScript->mainCamActive) {
			cam = MainCam;
		}
		else {
			cam = StarShipCam;
		}
	}

	MouseAim();

	if (((Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT)|| (Input::GetKey(KeyCode::KEY_D) == KeyState::KEY_REPEAT)
		|| (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_REPEAT) || (Input::GetKey(KeyCode::KEY_S) == KeyState::KEY_REPEAT)))
	{

		if (velk < maxVel) {
			velk += accel * dt;
		}

		moving = true;
		braking = false;
	}
	else
	{
		if (velk > 0)
		{
			velk -= brake * dt;
			braking = true;
		}
	}

	if (velk <= 0) {
		moving = false;
		braking = false;
		//brake = 0.05f;
		velk = 0.0f;
		movB = movF = movR = movL = false;

	}

	//MOVEMENT TO FORWARD
	/*if (Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT)
	{
		gameObject.GetTransform().Translate(cam.GetGameObject().GetTransform().GetForward() * velk);
		
	}*/
	if (Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT || (movB && braking))
	{
		API_Vector3 vecF;
		vecF.x = cam.GetGameObject().GetTransform().GetForward().x;
		vecF.y = 0;
		vecF.z = cam.GetGameObject().GetTransform().GetForward().z;

		if (Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT)
		{
			gameObject.GetTransform().Translate(vecF * velk);
			movB = true;
		}
		else if (movB && braking)
		{
			gameObject.GetTransform().Translate(vecF * velk);
		}
	}

	//MOVEMENT TO RIGHT
	/*if (Input::GetKey(KeyCode::KEY_D) == KeyState::KEY_REPEAT)
	{
		gameObject.GetTransform().Translate(cam.GetGameObject().GetTransform().GetLeft() * velk);
		
	}*/
	if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_REPEAT || (movL && braking))
	{
		API_Vector3 vecR;
		vecR.x = cam.GetGameObject().GetTransform().GetRight().x;
		vecR.y = 0;
		vecR.z = cam.GetGameObject().GetTransform().GetRight().z;

		if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_REPEAT)
		{
			gameObject.GetTransform().Translate(vecR * velk);
			movL = true;
		}
		else if (movL && braking)
		{
			gameObject.GetTransform().Translate(vecR * velk);
		}
	}
	//MOVEMENT TO BACKWARD
	/*if (Input::GetKey(KeyCode::KEY_S) == KeyState::KEY_REPEAT)
	{
		gameObject.GetTransform().Translate(cam.GetGameObject().GetTransform().GetBackward() * velk);
		
	}*/
	if (Input::GetKey(KeyCode::KEY_S) == KeyState::KEY_REPEAT || (movF && braking))
	{
		API_Vector3 vecB;
		vecB.x = cam.GetGameObject().GetTransform().GetBackward().x;
		vecB.y = 0;
		vecB.z = cam.GetGameObject().GetTransform().GetBackward().z;

		if (Input::GetKey(KeyCode::KEY_S) == KeyState::KEY_REPEAT)
		{
			gameObject.GetTransform().Translate(vecB * velk);
			movF = true;
		}
		else if (movF && braking)
		{
			gameObject.GetTransform().Translate(vecB * velk);
		}
	}

	//MOVEMENT TO LEFT
	/*if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_REPEAT)
	{
		gameObject.GetTransform().Translate(cam.GetGameObject().GetTransform().GetRight() * velk);
		
	}*/
	if (Input::GetKey(KeyCode::KEY_D) == KeyState::KEY_REPEAT || (movR && braking))
	{
		API_Vector3 vecL;
		vecL.x = cam.GetGameObject().GetTransform().GetLeft().x;
		vecL.y = 0;
		vecL.z = cam.GetGameObject().GetTransform().GetLeft().z;

		if (Input::GetKey(KeyCode::KEY_D) == KeyState::KEY_REPEAT)
		{
			gameObject.GetTransform().Translate(vecL * velk);
			movR = true;
		}
		else if (movR && braking)
		{
			gameObject.GetTransform().Translate(vecL * velk);
		}
	}

	//DASH
	if (Input::GetKey(KeyCode::KEY_LSHIFT) == KeyState::KEY_DOWN && dashActive == false) {
		dashActive = true;
	}
	if (dashActive) {
		Dash(dt);
	}
	else {
		dashCooldown -= dt;
		if (dashCooldown <= 0) {
			dashCooldown = 5.0f;
		}
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

void PlayerKeyboardMovement::Dash(float dt)
{
	startDash += dt;
	if (startDash <= endDash) {
		accel = 200.0f;
	}
	else {
		accel = 0.15f;
		velk = 0.0f;
		startDash = 0.0f;
		dashActive = false;
	}
}


