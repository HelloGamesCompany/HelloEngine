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
	dllFile = LoadLibrary(TEXT("HelloAPI.dll"));
}

LayerGame::~LayerGame()
{
}

void LayerGame::Start()
{
}

void LayerGame::PreUpdate()
{
	// Set hot reload boolean to false every preupdate to prevent hot realoding more than once at a time.
	_reloadedDLL = false;

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
		script.second->Start();
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
	if (_reloadedDLL)
		return;

	// Destroy every HellobEhavior in map
	for (auto& script : _behaviorScripts)
	{
		RELEASE(script.second);
	}

	// Free DLL
	FreeLibrary(dllFile);
	// Compile DLL
	
	// Load DLL
	dllFile = LoadLibrary(TEXT("HelloAPI.dll"));

	// Create every HElloBehavior in map
	for (int i = 0; i < _scriptComponents.size(); ++i)
	{
		CreateFunc create = (CreateFunc)GetProcAddress(dllFile, ("Create" + _scriptComponents[i]->scriptResource->className).c_str());

		uint randomUID = HelloUUID::GenerateUUID();
		_behaviorScripts[randomUID] = (HelloBehavior*)create();

		_scriptComponents[i]->scriptUID = randomUID;
	}

	_reloadedDLL = true;
}

void LayerGame::CleanUp()
{
}
