#include "Headers.h"	
#include "API_Game.h"
#include "ModuleLayers.h"
#include "Console.h"
#include "LayerGame.h"

API::API_GameObject API::Game::FindGameObject(const char* name)
{
	for (auto& gameObject : ModuleLayers::gameObjects)
	{
		if (gameObject.second->name == name)
		{
			API_GameObject apiGO;
			apiGO.SetGameObject(gameObject.second);
			return apiGO;
		}
	}
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

TO_API void API::Game::FindGameObjectsWithTag(const char* tag, API_GameObject* buffer, uint count)
{
	uint currentCount = 0;
	for (auto& gameObject : ModuleLayers::gameObjects)
	{
		if (gameObject.second->tag == tag)
		{
			API_GameObject apiGO;
			apiGO.SetGameObject(gameObject.second);
			*buffer = apiGO;
			++buffer;
			++currentCount;
			if (currentCount == count)
				return;
		}
	}
}
