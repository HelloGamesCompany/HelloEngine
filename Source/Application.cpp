#include "Headers.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "ModuleLayers.h"
#include "ModuleXML.h"
#include "ModuleFiles.h"

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
	file = new ModuleFiles();
	input = new ModuleInput(true);
	camera = new ModuleCamera3D(true);
	renderer3D = new ModuleRenderer3D(true);
	layers = new ModuleLayers();
	xml = new ModuleXML();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(file);
	AddModule(xml);
	AddModule(window);

	AddModule(input);
	AddModule(camera);

	AddModule(layers);

	// Renderer last!
	AddModule(renderer3D);

	//Set up frame rate
	XMLNode configNode = xml->GetConfigXML();
	frameCap = configNode.node.child("renderer").child("framerate").attribute("value").as_int(60);
	SetFPS(frameCap);

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

	XMLNode configNode = xml->GetConfigXML();
	configNode.node.child("renderer").child("framerate").attribute("value").set_value(frameCap);

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

std::string Application::GetEngineVersion()
{
	return "v0.1";
}
