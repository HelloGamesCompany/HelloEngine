#include "Tank.h"
HELLO_ENGINE_API_C Tank* CreateTank(ScriptToInspectorInterface* script)
{
	Tank* classInstance = new Tank();
	
	script->AddDragBoxTransform("Turret", &classInstance->turret);
	script->AddDragFloat("Moving speed", &classInstance->movingSpeed);
	return classInstance;
}

void Tank::Start()
{
	Engine::centerMouse = true; // This hides the mouse and centers on the screen (FPS style)
}

void Tank::Update()
{
	if (Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT)
	{
		Console::Log(std::to_string(movingSpeed));
		this->gameObject.GetTransform().Translate(this->gameObject.GetTransform().GetForward() * movingSpeed);
	}
}