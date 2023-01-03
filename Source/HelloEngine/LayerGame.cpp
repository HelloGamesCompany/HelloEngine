#include "Headers.h"
#include "LayerGame.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "ScriptComponent.h"
#include "ScriptToInspectorInterface.h"
#include "ModuleCommand.h"
#include "ModuleLayers.h"

std::map<uint, BehaviorScript> LayerGame::_behaviorScripts;
std::vector<ScriptComponent*> LayerGame::_scriptComponents;
bool LayerGame::_isPlaying = false;
bool LayerGame::_paused = false;
bool LayerGame::_update = false;
bool LayerGame::_oneFrame = false;
bool LayerGame::_needsReload = false;
HMODULE LayerGame::_dllFile;
time_t LayerGame::_dllChangeTime;
bool LayerGame::detectInput = true;
std::string LayerGame::currentScene = "";

typedef void* (*CreateFunc)(ScriptToInspectorInterface*);

LayerGame::LayerGame()
{
	_dllChangeTime = ModuleFiles::S_CheckFileLastModify(DLL_DIRECTORY);
	CopyFile(TEXT(DLL_DIRECTORY), TEXT("APILibrary/HelloAPI.dll"), FALSE);
	_dllFile = LoadLibrary("APILibrary/HelloAPI.dll");

	ModuleFiles::S_CreateScriptFile("Creationtest", "Assets/Scripts/");
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
	if (temp != _dllChangeTime)
	{
		_dllChangeTime = temp;
		_needsReload = true;
	}

	if (_needsReload && !_isPlaying)
	{
		S_HotReload();
	}

	if ((!_isPlaying || _paused) && !_oneFrame)
	{
		_update = false;
		return;
	}
	_oneFrame = false;
	_update = true;
	
	// Update time.
	EngineTime::UpdateRealTime();
	EngineTime::UpdateGameTime();
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

void LayerGame::S_Play()
{
	ModuleCommand::_canUseCommand = false;
	// TODO: Save scene.
	currentScene = Application::Instance()->xml->GetConfigXML().FindChildBreadth("currentScene").node.attribute("value").as_string();
	ModuleResourceManager::S_SerializeScene(ModuleLayers::rootGameObject);

	EngineTime::Reset();
	EngineTime::Start();
	_isPlaying = true;

	for (auto& behaviorScript : _behaviorScripts)
	{
		if (behaviorScript.second.active)
			behaviorScript.second.script->Start();
	}
}

void LayerGame::S_Stop()
{
	ModuleCommand::_canUseCommand = true;
	EngineTime::Reset();
	_isPlaying = false;
	_paused = false;
	// TODO: Reload scene.
	ModuleLayers::S_RequestLoadScene(currentScene);

}

void LayerGame::S_Pause()
{
	//if (!_isPlaying)
	//	return;

	_paused = !_paused;
	if (!_paused)
		EngineTime::Start();
}

void LayerGame::S_OneFrame()
{
	if (!_isPlaying || !_paused) return;
	EngineTime::Start();
	_oneFrame = true;
}

void LayerGame::S_AddScriptComponent(ScriptComponent* component)
{
	_scriptComponents.push_back(component);
	S_CreateBehaviorScript(component);
}

void LayerGame::S_RemoveScriptComponent(ScriptComponent* component)
{
	for (int i = 0; i < _scriptComponents.size(); ++i)
	{
		if (component == _scriptComponents[i])
		{
			_scriptComponents.erase(_scriptComponents.begin() + i);
			S_DestroyBehaviorScript(component);
			return;
		}
	}
	LOG("Didn't found component in script components vector in layer game.");
}

void LayerGame::S_HotReload()
{
	// Destroy every HellobEhavior in map
	for (int i = 0; i < _scriptComponents.size(); ++i)
	{
		S_DestroyBehaviorScript(_scriptComponents[i]);
		_scriptComponents[i]->SaveInspectorFields();
		_scriptComponents[i]->DestroyInspectorFields();
	}

	_behaviorScripts.clear();

	// Free DLL
	FreeLibrary(_dllFile);

	// Move new dll into ouptut to use.
	while (CopyFile(TEXT(DLL_DIRECTORY), TEXT("APILibrary/HelloAPI.dll"), FALSE) == FALSE)
	{
		printf("Changing DLL.");
	}

	// Load DLL
	_dllFile = LoadLibrary(TEXT("APILibrary/HelloAPI.dll"));

	// Create every HElloBehavior in map
	for (int i = 0; i < _scriptComponents.size(); ++i)
	{
		S_CreateBehaviorScript(_scriptComponents[i]);
		_scriptComponents[i]->LoadInspectorFields();
	}

	_needsReload = false;
}

void LayerGame::CleanUp()
{
	for (auto& script : _behaviorScripts)
	{
		RELEASE(script.second.script);
	}
	FreeLibrary(_dllFile);
}

void LayerGame::S_CreateBehaviorScript(ScriptComponent* component)
{
	if (component->scriptResource == nullptr)
		return;

	CreateFunc create = (CreateFunc)GetProcAddress(_dllFile, ("Create" + component->scriptResource->className).c_str());

	uint randomUID = HelloUUID::GenerateUUID();
	_behaviorScripts[randomUID].script = (HelloBehavior*)create(component);
	_behaviorScripts[randomUID].active = component->IsEnabled();

	component->scriptUID = randomUID;
}

void LayerGame::S_DestroyBehaviorScript(ScriptComponent* component)
{
	if (component->scriptUID != 0)
		_behaviorScripts.erase(component->scriptUID);
}
