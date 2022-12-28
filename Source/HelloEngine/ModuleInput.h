#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"
#include "Globals.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	ModuleInput(bool start_enabled = true);

	~ModuleInput();

	bool Init();

	UpdateStatus PreUpdate();

	bool CleanUp();

	static KEY_STATE S_GetKey(int id)
	{
		return _keyboard[id];
	}

	static KEY_STATE S_GetMouseButton(int id)
	{
		return _mouse_buttons[id];
	}

	static void S_HandleMouseButton(int id)
	{
		_mouse_buttons[id] = KEY_STATE::KEY_IDLE;
	}

	static int S_GetMouseX()
	{
		return _mouse_x;
	}

	static int S_GetMouseY()
	{
		return _mouse_y;
	}

	static int S_GetMouseZ()
	{
		return _mouse_z;
	}

	static int S_GetMouseXMotion()
	{
		return _mouse_x_motion;
	}

	static int S_GetMouseYMotion()
	{
		return _mouse_y_motion;
	}

	static void S_AddOnDropListener(std::function<void(std::string)> func);

	static void S_ClearOnDropListener();

private:
	static KEY_STATE* _keyboard;

	static KEY_STATE _mouse_buttons[MAX_MOUSE_BUTTONS];

	static int _mouse_x;

	static int _mouse_y;

	static int _mouse_z;

	static int _mouse_x_motion;

	static int _mouse_y_motion;

	static std::function<void(std::string)> _dropEvent;
};

#endif // !__MODULEINPUT_H__