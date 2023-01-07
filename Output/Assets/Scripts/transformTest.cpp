#include "transformTest.h"
#include "BulletBehavior.h"

HELLO_ENGINE_API_C transformTest* CreatetransformTest(ScriptToInspectorInterface* script)
{
	transformTest* classInstance = new transformTest();
	script->AddDragBoxTransform("transform dragging test", &classInstance->transfromTestVariable);
	script->AddDragBoxMeshRenderer("meshTest", &classInstance->meshRendererTest);

	return classInstance;
}

void transformTest::Start()
{
	newGameObjectTest = Game::CreateGameObject("Name", "Tag");
	gameObjectLife = 8.0f;
}

void transformTest::Update()
{
	gameObjectLife -= Time::GetDeltaTime();

	if (gameObjectLife <= 0 && newGameObjectTest.IsAlive())
	{
		newGameObjectTest.Destroy();
	}

	// Create bullet
	if (Input::GetKey(KeyCode::KEY_N) == KeyState::KEY_DOWN)
	{
		// Create game object
		API_GameObject newBullet = Game::CreateGameObject("Bullet", "Projectile");
		newBullet.GetTransform().SetPosition(this->gameObject.GetTransform().GetPosition());
		// Add behavior
		BulletBehavior* bulletScript = (BulletBehavior*)newBullet.AddScript("BulletBehavior");
		// Add mesh renderer, copied from another mesh renderer properties.
		newBullet.AddMeshRenderer(this->meshRendererTest);

		// Set direction of the bullet
		if (bulletScript != nullptr)
			bulletScript->direction = transfromTestVariable.GetDown();
	}

	// Load new scene
	if (Input::GetKey(KeyCode::KEY_L) == KeyState::KEY_REPEAT)
	{
		Scene::LoadScene("bakerhouse.HScene");
	}

	// move object
	if (Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Translate(transfromTestVariable.GetDown());
	}
	if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Translate(transfromTestVariable.GetRight());
	}
	if (Input::GetKey(KeyCode::KEY_S) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Translate(transfromTestVariable.GetUp());
	}
	if (Input::GetKey(KeyCode::KEY_D) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Translate(transfromTestVariable.GetLeft());
	}
	if (Input::GetKey(KeyCode::KEY_Q) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Rotate(0, 0, 1);
	}
	if (Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Rotate(0, 0, -1);
	}
	if (Input::GetKey(KeyCode::KEY_R) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.SetPosition(0, 0, 0);
	}
}