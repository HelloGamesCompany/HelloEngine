#include "PlayerKeyboardMovement.h"
HELLO_ENGINE_API_C PlayerKeyboardMovement* CreatePlayerKeyboardMovement(ScriptToInspectorInterface* script)
{
	PlayerKeyboardMovement* classInstance = new PlayerKeyboardMovement();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void PlayerKeyboardMovement::Start()
{

}
void PlayerKeyboardMovement::Update()
{
	float vel = 0.2;

	if (Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT)
	{
		gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward()* vel );
	}
	if (Input::GetKey(KeyCode::KEY_D) == KeyState::KEY_REPEAT)
	{
		gameObject.GetTransform().Translate(gameObject.GetTransform().GetLeft() * vel);
	}
	if (Input::GetKey(KeyCode::KEY_S) == KeyState::KEY_REPEAT)
		{
			gameObject.GetTransform().Translate(gameObject.GetTransform().GetBackward() * vel);
		}
	if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_REPEAT)
		{
			gameObject.GetTransform().Translate(gameObject.GetTransform().GetRight() * vel);
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