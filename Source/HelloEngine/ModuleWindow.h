#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	static void S_SetTitle(const char* title);
	static void S_AddTitleExtraInfo(const std::string& context);
	static void S_SetBrightness(float bright);
	static int S_GetMaxRefreshRate();

public:
	//The window we'll be rendering to
	static SDL_Window* window;

	static int width;

	static int height;

	static float brightness;

private:
	//The surface contained by the window
	static SDL_Surface* _screen_surface;
	
	static std::string _title;
};

#endif // __ModuleWindow_H__