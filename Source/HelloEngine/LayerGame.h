#pragma once
#include "Layer.h"
#include "MeshRenderComponent.h"
#include "GameObject.h"

// DLL-----
#include "HelloBehavior.h"

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

	void TO_API AddScript(uint UID, HelloBehavior* script);

	// Should be called before reloading DLL.
	void DestroyScripts();

	void CleanUp() override;
private:
	std::map<uint, HelloBehavior*> _scripts;
	bool _isPlaying = false;
	bool _paused = false;
	bool _update = false;
	bool _oneFrame = false;

	friend class LayerEditor;
};

