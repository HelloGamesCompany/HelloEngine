#include "Headers.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"
#include "ModuleXML.h"

Application* Application::app = nullptr;

Application::Application()
{
}

Application::~Application()
{
}

bool Application::Init()
{
	window = new ModuleWindow(true);
	input = new ModuleInput(true);
	camera = new ModuleCamera3D(true);
	renderer3D = new ModuleRenderer3D(true);
	editor = new ModuleEditor(true);
	xml = new ModuleXML();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(xml);
	AddModule(window);

	AddModule(input);
	AddModule(camera);

	AddModule(editor);

	// Renderer last!
	AddModule(renderer3D);



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
	return true;
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

Application* Application::Instance()
{
	if (app == nullptr) app = new Application();

	return app;
}
