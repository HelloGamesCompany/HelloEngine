#pragma once
#include "Globals.h"
#include "API_GameObject.h"

namespace API
{
	static class Game
	{
	public:
		TO_API static void FindGameObject(const char* name);
		TO_API static API_GameObject CreateGameObject(const char* name, const char* tag, API_GameObject* parent = nullptr);

		TO_API static void ExitApplication();
	};
}