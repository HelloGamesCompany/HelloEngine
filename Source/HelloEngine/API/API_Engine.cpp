#include "Headers.h"
#include "API_Engine.h"
#include "ModuleWindow.h"
#include "LayerGame.h"

bool API::Engine::hideMouse = false;
bool API::Engine::centerMouse = true;
bool API::Engine::isMouseCentered = false;
int API::Engine::gameWindowX;
int API::Engine::gameWindowY;
int API::Engine::gameWindowWidth;
int API::Engine::gameWindowHeight;

void API::Engine::ApplyEngineProperties()
{
	// Mouse hide
	SDL_ShowCursor(!hideMouse);
	isMouseCentered = centerMouse;
	SDL_SetRelativeMouseMode((SDL_bool)centerMouse);
}

void API::Engine::EnginePropertiesUpdate()
{

}

void API::Engine::UnApplyEngineProperties()
{
	// Mouse hide
	SDL_ShowCursor(1);
	isMouseCentered = false;
	SDL_SetRelativeMouseMode(SDL_bool::SDL_FALSE);
}
