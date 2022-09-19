#ifndef __MODULE_H__
#define __MODULE_H__

#include "Globals.h"

class PhysBody3D;
class Application;

class Module
{
private:
	bool enabled = false;

public:
	Application* App = nullptr;

public:
	Module();

	Module(Application* parent, bool start_enabled = true);

	virtual ~Module();

	virtual bool Init();

	virtual bool Start();

	virtual UpdateStatus PreUpdate();

	virtual UpdateStatus Update();

	virtual UpdateStatus PostUpdate();

	virtual bool CleanUp();
};

#endif // !__MODULE_H__