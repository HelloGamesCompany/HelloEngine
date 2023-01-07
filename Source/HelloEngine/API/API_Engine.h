#pragma once
#include "Globals.h"

class ImWindowGame;

namespace API
{
	static class TO_API Engine
	{
	public:
		static bool hideMouse;
		static bool centerMouse;
		static bool isMouseCentered;
		static int gameWindowX;
		static int gameWindowY;
		static int gameWindowWidth;
		static int gameWindowHeight;

	private:
		static void EnginePropertiesUpdate();
		static void ApplyEngineProperties();
		static void UnApplyEngineProperties();

		friend class LayerGame;
		friend class ImWindowGame;
	};
}