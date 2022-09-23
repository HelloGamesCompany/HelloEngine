#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Timer.hpp"
#include <vector>

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleCamera3D;
class ModuleRenderer3D;
class ModuleEditor;

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleEditor* editor = nullptr;

	float fps = 1.0f/60.0f;
	Timer	timer;

private:
	float	dt = 0;
	std::vector<Module*> list_modules;
	static Application* app;
public:
	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
	bool CleanUp();

	static Application* Instante();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

#endif // !__APPLICATION_H__