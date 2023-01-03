#include "transformTest.h"
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

	// Load new scene
	if (Input::GetKey(KeyCode::KEY_L) == KeyState::KEY_REPEAT)
	{
		Scene::LoadScene("bakerhouse.HScene");
	}

	// move object
	if (Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Translate(0, 0, 0.2);
	}
	if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Translate(0.2, 0, 0);
	}
	if (Input::GetKey(KeyCode::KEY_S) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Translate(0, 0, -0.2);
	}
	if (Input::GetKey(KeyCode::KEY_D) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Translate(-0.2, 0, 0);
	}
	if (Input::GetKey(KeyCode::KEY_Q) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Rotate(0, 1, 0);
	}
	if (Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Rotate(0, -1, 0);
	}
	if (Input::GetKey(KeyCode::KEY_R) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.SetPosition(0, 0, 0);
	}
}