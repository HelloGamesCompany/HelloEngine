#pragma once
#include "Globals.h"
#include "API_GameObject.h"

namespace API
{
	static class Game
	{
	public:
		TO_API static API_GameObject FindGameObject(const char* name);
		TO_API static API_GameObject CreateGameObject(const char* name, const char* tag, API_GameObject* parent = nullptr);

		TO_API static void ExitApplication();

		/// <param name="tag">Tag to be searched</param>
		/// <param name="buffer">The first element of an array of API_GameObject</param>
		/// <param name="count">How many game objects do you want to find</param>
		/// <returns></returns>
		TO_API static void FindGameObjectsWithTag(const char* tag, API_GameObject* buffer, uint count);
	};
}