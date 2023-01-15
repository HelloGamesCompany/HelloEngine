#include "TankBehavior.h"
#include "BulletBehavior.h"

HELLO_ENGINE_API_C TankBehavior* CreateTankBehavior(ScriptToInspectorInterface* script)
{
	TankBehavior* classInstance = new TankBehavior();
	script->AddDragFloat("Moving speed", &classInstance->movingSpeed);
	script->AddDragBoxTransform("Turret", &classInstance->turret);
	script->AddDragBoxMeshRenderer("Bullet", &classInstance->bulletMesh);

	return classInstance;
}

void TankBehavior::Start()
{
}

void TankBehavior::Update()
{
	// Create bullets

	if (Input::GetKey(KeyCode::KEY_SPACE) == KeyState::KEY_DOWN)
	{
		// Create game object
		API_GameObject newBullet = Game::CreateGameObject("Bullet", "Projectile");
		newBullet.GetTransform().SetPosition(this->gameObject.GetTransform().GetGlobalPosition() + API_Vector3(0,1.5f,0));
		// Add behavior
		BulletBehavior* bulletScript = (BulletBehavior*)newBullet.AddScript("BulletBehavior");
		// Add mesh renderer, copied from another mesh renderer properties.
		newBullet.AddMeshRenderer(this->bulletMesh);

		// Set direction of the bullet
		if (bulletScript != nullptr)
		{
			bulletScript->direction = gameObject.GetTransform().GetForward();
			API_Vector3 rotation = API_Vector3(0, gameObject.GetTransform().GetLocalRotation().y, 0);
			Console::Log("X:" + std::to_string(gameObject.GetTransform().GetLocalRotation().x));
			Console::Log("Y:" + std::to_string(gameObject.GetTransform().GetLocalRotation().y));
			Console::Log("Z:" + std::to_string(gameObject.GetTransform().GetLocalRotation().z));
			bulletScript->gameObject.GetTransform().SetRotation(rotation);
		}
	}


	// Movement
	if (Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT)
	{
		// Move forward
		this->gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * movingSpeed);
	}
	if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_REPEAT)
	{
		// Move left
		this->gameObject.GetTransform().Translate(gameObject.GetTransform().GetLeft() * movingSpeed);
	}
	if (Input::GetKey(KeyCode::KEY_S) == KeyState::KEY_REPEAT)
	{
		// Move backward
		this->gameObject.GetTransform().Translate(gameObject.GetTransform().GetBackward() * movingSpeed);
	}
	if (Input::GetKey(KeyCode::KEY_D) == KeyState::KEY_REPEAT)
	{
		// Move right
		this->gameObject.GetTransform().Translate(gameObject.GetTransform().GetRight() * movingSpeed);
	}

	if (Input::GetKey(KeyCode::KEY_Q) == KeyState::KEY_REPEAT)
	{
		// Rotate right
		this->gameObject.GetTransform().Rotate(0, 1, 0);
	}
	if (Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_REPEAT)
	{
		// Move left
		this->gameObject.GetTransform().Rotate(0, -1, 0);
	}

}