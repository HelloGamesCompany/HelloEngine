#include "Headers.h"
#include "LayerGame.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "ScriptComponent.h"

typedef void* (*CreateFunc)();

LayerGame::LayerGame()
{
	dllChangeTime = ModuleFiles::S_CheckFileLastModify(DLL_DIRECTORY);
	CopyFile(TEXT(DLL_DIRECTORY), TEXT("APILibrary/HelloAPI.dll"), FALSE);
	dllFile = LoadLibrary("APILibrary/HelloAPI.dll");

	// TEMPORAL
	CreateFunc createTest = (CreateFunc)GetProcAddress(dllFile, "CreateTest");
	_behaviorScripts[0] = (HelloBehavior*)createTest();

	//ModuleFiles::S_CreateScriptFile("NewScript", "Scripts/");

}

LayerGame::~LayerGame()
{
}

void LayerGame::Start()
{
}

void LayerGame::PreUpdate()
{
	// Check if saved time and change time is not equal.
	time_t temp = ModuleFiles::S_CheckFileLastModify("HelloAPI/bin/Release/HelloAPI.dll");
	if (temp != dllChangeTime)
	{
		dllChangeTime = temp;
		_needsReload = true;
	}

	if (_needsReload && !_isPlaying)
	{
		HotReload();
	}

	if ((!_isPlaying || _paused) && !_oneFrame)
	{
		_update = false;
		return;
	}
	_oneFrame = false;
	_update = true;
	
	// Update time.
	Time::UpdateRealTime();
	Time::UpdateGameTime();

	for (auto& script : _behaviorScripts)
	{
		script.second->Update();
	}
}

void LayerGame::Update()
{
	if (!_update)
		return;
}

void LayerGame::PostUpdate()
{
	if (!_update)
		return;
}

void LayerGame::Play()
{
	// TODO: Save scene.
	Time::Reset();
	Time::Start();
	_isPlaying = true;

	for (auto& script : _behaviorScripts)
	{
		try
		{
			script.second->Start();
		}
		catch (...)
		{
			LOG("Error in Start ");
		}
	}
}

void LayerGame::Stop()
{
	Time::Reset();
	_isPlaying = false;
	_paused = false;
	// TODO: Reload scene.
}

void LayerGame::Pause()
{
	//if (!_isPlaying)
	//	return;

	_paused = !_paused;
	if (!_paused)
		Time::Start();
}

void LayerGame::OneFrame()
{
	if (!_isPlaying || !_paused) return;
	Time::Start();
	_oneFrame = true;
}

void LayerGame::AddScriptComponent(ScriptComponent* component)
{
	_scriptComponents.push_back(component);
}

void LayerGame::RemoveScriptComponent(ScriptComponent* component)
{
	for (int i = 0; i < _scriptComponents.size(); ++i)
	{
		if (component == _scriptComponents[i])
		{
			_scriptComponents.erase(_scriptComponents.begin() + i);
			return;
		}
	}
	LOG("Didn't found component in script components vector in layer game.");
}

void LayerGame::HotReload()
{
	// Destroy every HellobEhavior in map
	for (auto& script : _behaviorScripts)
	{
		RELEASE(script.second);
	}
	_behaviorScripts.clear();
	// Free DLL
	FreeLibrary(dllFile);

	// Move new dll into ouptut to use.
	while (CopyFile(TEXT(DLL_DIRECTORY), TEXT("APILibrary/HelloAPI.dll"), FALSE) == FALSE)
	{
		printf("Changing DLL.");
	}

	// Load DLL
	dllFile = LoadLibrary(TEXT("APILibrary/HelloAPI.dll"));

	// Create every HElloBehavior in map
	for (int i = 0; i < _scriptComponents.size(); ++i)
	{
		CreateFunc create = (CreateFunc)GetProcAddress(dllFile, ("Create" + _scriptComponents[i]->scriptResource->className).c_str());

		uint randomUID = HelloUUID::GenerateUUID();
		_behaviorScripts[randomUID] = (HelloBehavior*)create();

		_scriptComponents[i]->scriptUID = randomUID;
	}

	CreateFunc createTest = (CreateFunc)GetProcAddress(dllFile, "CreateTest");
	_behaviorScripts[0] = (HelloBehavior*)createTest();

	_needsReload = false;
}

void LayerGame::CleanUp()
{
	for (auto& script : _behaviorScripts)
	{
		RELEASE(script.second);
	}
	FreeLibrary(dllFile);
}
