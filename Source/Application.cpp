#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"

Application::Application()
{
	window = new ModuleWindow(this, true);
	input = new ModuleInput(this, true);
	camera = new ModuleCamera3D(this, true);
	renderer3D = new ModuleRenderer3D(this, true);
	editor = new ModuleEditor(true);
	
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	
	AddModule(input);
	AddModule(camera);
	
	// Renderer last!
	AddModule(renderer3D);

	AddModule(editor);
}

Application::~Application()
{
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	for (int i = 0, count = list_modules.size() ; i <count ; i++)
	{
		list_modules[i]->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	for (int i = 0, count = list_modules.size(); i < count; i++)
	{
		list_modules[i]->Start();
	}

	//ms_timer.Start();
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	//ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
UpdateStatus Application::Update()
{
	UpdateStatus ret = UpdateStatus::UPDATE_CONTINUE;
	PrepareUpdate();

	for (int i = 0, count = list_modules.size(); i < count && ret == UpdateStatus::UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->PreUpdate();
	}

	for (int i = 0, count = list_modules.size(); i < count && ret == UpdateStatus::UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->Update();
	}

	for (int i = 0, count = list_modules.size(); i < count && ret == UpdateStatus::UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->PostUpdate();
	}

	dt = timer.getDeltaTime();

	if (dt < fps)
	{
		float sleepTime = (fps - dt) * 1000;
		Sleep(sleepTime);
	}

	timer.Reset();

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (int i = list_modules.size() -1; i >= 0 && ret; i--)
	{
		ret = list_modules[i]->CleanUp();
	}

	for (int i = list_modules.size() - 1; i >= 0; i--)
	{
		RELEASE(list_modules[i]);
	}

	return ret;
}
