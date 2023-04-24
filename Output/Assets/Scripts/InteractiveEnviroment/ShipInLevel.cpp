#include "ShipInLevel.h"
#include "../FadeToBlack.h"

HELLO_ENGINE_API_C ShipInLevel* CreateShipInLevel(ScriptToInspectorInterface* script)
{
	ShipInLevel* classInstance = new ShipInLevel();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	//script->AddDragInt("Num of Ships", &classInstance->numOfShips);
	script->AddDragBoxGameObject("Black Image", &classInstance->fadeToBlackGO);
	script->AddDragFloat("timer", &classInstance->timerTotp);

	return classInstance;
}

void ShipInLevel::Start()
{
	//ships.resize(numOfShips);

	//Game::FindGameObjectsWithTag("Ship", &ships[0], numOfShips);

	fadeToBlackRef = (FadeToBlack*)fadeToBlackGO.GetScript("FadeToBlack");
	
}
void ShipInLevel::Update()
{



}

void ShipInLevel::OnCollisionStay(API::API_RigidBody other)
{

    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
		if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN)
		{
			fadeToBlackRef->fadeToBlack = true;

		}
		if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_REPEAT)
		{
			timerTotp += Time::GetDeltaTime();

			if (timerTotp >= 1.2f)
			{

				Scene::LoadScene("SpaceshipHUB_Scene.HScene");

				timerTotp = 0.0f;
			}
		}
    }

}
