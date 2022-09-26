#include "Headers.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "XMLNode.h"
#include "ModuleXML.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	window = nullptr;
	screen_surface = nullptr;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		XMLNode configNode = app->xml->GetConfigXML();
		//Create window
		width = configNode.node.child("window").child("width").attribute("value").as_int(1280);
		height = configNode.node.child("window").child("height").attribute("value").as_int(720);
		brightness = configNode.node.child("window").child("brightness").attribute("value").as_float(1.0f);
		
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		SetBrightness(brightness);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	SetTitle("Hello Engine");

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	XMLNode configNode = app->xml->GetConfigXML();

	configNode.node.child("window").child("width").attribute("value").set_value(width);
	configNode.node.child("window").child("height").attribute("value").set_value(height);;
	configNode.node.child("window").child("brightness").attribute("value").set_value(brightness);

	configNode.Save();

	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetBrightness(float bright)
{
	bright = MAX(0.2f, MIN(1.0f, bright)); // check if brightness is between 0.2 and 1.0 and assign it to the closest valid value.

	SDL_SetWindowBrightness(window, bright);
}

int ModuleWindow::GetMaxRefreshRate()
{
	SDL_DisplayMode display;
	SDL_GetDisplayMode(0, 0, &display);

	return display.refresh_rate;
}
