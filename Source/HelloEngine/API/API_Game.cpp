#include "Headers.h"	
#include "API_Game.h"
#include "ModuleLayers.h"
#include "Console.h"
#include "LayerGame.h"

void API::Game::FindGameObject(const char* name)
{
}

API::API_GameObject API::Game::CreateGameObject(const char* name, const char* tag, API_GameObject* parent)
{
	GameObject* newGameObject = nullptr;

	if (parent == nullptr)
		newGameObject = new GameObject(ModuleLayers::rootGameObject, name, tag);
	else
	{
		GameObject* newParent = parent->GetGameObject();
		if (newParent != nullptr)
			newGameObject = new GameObject(newParent, name, tag);
		else
		{
			Engine::Console::S_Log("Tried to create a new GameObject with a null parent using Game::CreateGameObject. Parent ignored.", LogType::WARNING);
			newGameObject = new GameObject(ModuleLayers::rootGameObject, name, tag);
		}
	}

	API_GameObject newAPIGameObject;
	newAPIGameObject.SetGameObject(newGameObject);

	return newAPIGameObject;
}

TO_API void API::Game::ExitApplication()
{
#ifdef STANDALONE
	LayerGame::S_Stop();
#else
	Application::Instance()->Exit();
#endif // STANDALONE

}
