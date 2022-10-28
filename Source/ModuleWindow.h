#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "External/SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void SetBrightness(float bright);
	int GetMaxRefreshRate();

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	int width = 0, height = 0;
	float brightness = 1.0f;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;
};

#endif // __ModuleWindow_H__