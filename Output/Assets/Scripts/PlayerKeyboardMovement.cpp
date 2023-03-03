#include "PlayerKeyboardMovement.h"
HELLO_ENGINE_API_C PlayerKeyboardMovement* CreatePlayerKeyboardMovement(ScriptToInspectorInterface* script)
{
	PlayerKeyboardMovement* classInstance = new PlayerKeyboardMovement();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragFloat("Velocity", &classInstance->vel);

	return classInstance;
}

void PlayerKeyboardMovement::Start()
{

}
void PlayerKeyboardMovement::Update()
{
	//float vel = 0.2;

	if (Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT)
	{
		//gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward()* vel );
		gameObject.GetTransform().Translate((0,0,1.0f)*vel);
		Console::Log("WWWWWWW");
	}
	if (Input::GetKey(KeyCode::KEY_D) == KeyState::KEY_REPEAT)
	{
		//gameObject.GetTransform().Translate(gameObject.GetTransform().GetLeft() * vel);
		gameObject.GetTransform().Translate((-1.0f, 0, 0) * vel);
		Console::Log("DDDDDDDDDD");
	}
	if (Input::GetKey(KeyCode::KEY_S) == KeyState::KEY_REPEAT)
		{
			//gameObject.GetTransform().Translate(gameObject.GetTransform().GetBackward() * vel);
			gameObject.GetTransform().Translate((0, 0, -1.0f) * vel);
			Console::Log("SSSSSSSSSSSS");
		}
	if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_REPEAT)
		{
			//gameObject.GetTransform().Translate(gameObject.GetTransform().GetRight() * vel);
			gameObject.GetTransform().Translate((1.0f, 0, 0) * vel);
			Console::Log("AAAAAAAA");
		}
	
	/*if (Input::GetMouseXMotion() < 0)
	{
		gameObject.GetTransform().Rotate(0, 1, 0);
		
	}
	if (Input::GetMouseXMotion() > 0)
	{
		gameObject.GetTransform().Rotate(0, -1, 0);
	}*/


}