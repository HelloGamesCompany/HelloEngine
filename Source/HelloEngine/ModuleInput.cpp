#include "Headers.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "imgui_impl_sdl.h"

#define MAX_KEYS 300

// Init static variables
KEY_STATE* ModuleInput::_keyboard = nullptr;
KEY_STATE ModuleInput::_mouse_buttons[MAX_MOUSE_BUTTONS];
int ModuleInput::_mouse_x = 0;
int ModuleInput::_mouse_y = 0;
int ModuleInput::_mouse_z = 0;
int ModuleInput::_mouse_x_motion = 0;
int ModuleInput::_mouse_y_motion = 0;
std::function<void(std::string)> ModuleInput::_dropEvent = nullptr;

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	_keyboard = new KEY_STATE[MAX_KEYS];
	memset(_keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(_mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	RELEASE_ARRAY(_keyboard);
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");

	bool ret = true;
	
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
UpdateStatus ModuleInput::PreUpdate()
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(_keyboard[i] == KEY_IDLE)
				_keyboard[i] = KEY_DOWN;
			else
				_keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(_keyboard[i] == KEY_REPEAT || _keyboard[i] == KEY_DOWN)
				_keyboard[i] = KEY_UP;
			else
				_keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&_mouse_x, &_mouse_y);

	_mouse_x /= SCREEN_SIZE;
	_mouse_y /= SCREEN_SIZE;
	_mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(_mouse_buttons[i] == KEY_IDLE)
				_mouse_buttons[i] = KEY_DOWN;
			else
				_mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(_mouse_buttons[i] == KEY_REPEAT || _mouse_buttons[i] == KEY_DOWN)
				_mouse_buttons[i] = KEY_UP;
			else
				_mouse_buttons[i] = KEY_IDLE;
		}
	}

	_mouse_x_motion = _mouse_y_motion = 0;

	bool quit = false;

	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
#ifdef STANDALONE
		ImGui_ImplSDL2_ProcessEvent(&e);
#endif // STANDALONE

		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			{
				_mouse_z = e.wheel.y;
			}
			break;

			case SDL_MOUSEMOTION:
			{
				_mouse_x = e.motion.x / SCREEN_SIZE;
				_mouse_y = e.motion.y / SCREEN_SIZE;

				_mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
				_mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			}
			break;

			case SDL_QUIT:
			{
				quit = true;
			}
			break;

			case (SDL_DROPFILE):
			{
				if (_dropEvent) 
					_dropEvent(e.drop.file);
				
				// Free dropped_filedir memory
				SDL_free(e.drop.file);    
				break;
			}

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					app->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}

	if(quit == true)
		return UpdateStatus::UPDATE_STOP;

	return UpdateStatus::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::S_AddOnDropListener(std::function<void(std::string)> func)
{
	_dropEvent = func;
}

void ModuleInput::S_ClearOnDropListener()
{
	_dropEvent = nullptr;
}