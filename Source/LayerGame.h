#pragma once
#include "Layer.h"
#include "MeshRenderComponent.h"
#include "GameObject.h"

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

	void CleanUp() override;
private:
	bool _isPlaying = false;
	bool _paused = false;
	bool _update = false;
	bool _oneFrame = false;

	friend class LayerEditor;
};

