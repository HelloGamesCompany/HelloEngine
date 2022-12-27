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
	_behaviorScripts[0].script = (HelloBehavior*)createTest();

	ModuleFiles::S_CreateScriptFile("NewScript", "Assets/Scripts/");
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
	time_t temp = ModuleFiles::S_CheckFileLastModify(DLL_DIRECTORY);
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
}

void LayerGame::Update()
{
	if (!_update)
		return;

	for (auto& behaviorScript : _behaviorScripts)
	{
		if (behaviorScript.second.active)
			behaviorScript.second.script->Update();
	}
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

	for (auto& behaviorScript : _behaviorScripts)
	{
		if (behaviorScript.second.active)
			behaviorScript.second.script->Start();
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
	CreateBehaviorScript(component);
}

void LayerGame::RemoveScriptComponent(ScriptComponent* component)
{
	for (int i = 0; i < _scriptComponents.size(); ++i)
	{
		if (component == _scriptComponents[i])
		{
			_scriptComponents.erase(_scriptComponents.begin() + i);
			DestroyBehaviorScript(component);
			return;
		}
	}
	LOG("Didn't found component in script components vector in layer game.");
}

void LayerGame::HotReload()
{
	// Destroy every HellobEhavior in map
	for (auto& behaviorScript : _behaviorScripts)
	{
		RELEASE(behaviorScript.second.script);
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
		CreateBehaviorScript(_scriptComponents[i]);
	}

	CreateFunc createTest = (CreateFunc)GetProcAddress(dllFile, "CreateTest");
	_behaviorScripts[0].script = (HelloBehavior*)createTest();

	_needsReload = false;
}

void LayerGame::CleanUp()
{
	for (auto& script : _behaviorScripts)
	{
		RELEASE(script.second.script);
	}
	FreeLibrary(dllFile);
}

void LayerGame::CreateBehaviorScript(ScriptComponent* component)
{
	if (component->scriptResource == nullptr)
		return;

	CreateFunc create = (CreateFunc)GetProcAddress(dllFile, ("Create" + component->scriptResource->className).c_str());

	uint randomUID = HelloUUID::GenerateUUID();
	_behaviorScripts[randomUID].script = (HelloBehavior*)create();
	_behaviorScripts[randomUID].active = component->IsEnabled();

	component->scriptUID = randomUID;
}

void LayerGame::DestroyBehaviorScript(ScriptComponent* component)
{
	if (component->scriptUID != 0)
		_behaviorScripts.erase(component->scriptUID);
}
