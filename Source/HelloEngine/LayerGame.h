#pragma once
#include "Layer.h"
#include "MeshRenderComponent.h"
#include "GameObject.h"
#include "HelloBehavior.h"

#ifdef _DEBUG
#define DLL_DIRECTORY "HelloAPI/bin/Debug/HelloAPI.dll"
#else
#define DLL_DIRECTORY "HelloAPI/bin/Release/HelloAPI.dll"
#endif

class ScriptComponent;

class LayerGame : public Layer
{
public:
	LayerGame();
	~LayerGame();

	void Start() override;

	void PreUpdate() override;
	void Update() override;
	void PostUpdate() override;

	void Play();
	void Stop();
	void Pause();
	void OneFrame();

	void AddScriptComponent(ScriptComponent* component);
	void RemoveScriptComponent(ScriptComponent* component);

	void HotReload();

	void CleanUp() override;

private:

	std::map<uint, HelloBehavior*> _behaviorScripts;
	std::vector<ScriptComponent*> _scriptComponents;
	bool _isPlaying = false;
	bool _paused = false;
	bool _update = false;
	bool _oneFrame = false;
	bool _needsReload = false;
	HMODULE dllFile;
	time_t dllChangeTime;

	friend class LayerEditor;
};

