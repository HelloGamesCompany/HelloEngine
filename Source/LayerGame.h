#pragma once
#include "Layer.h"
#include "MeshRenderComponent.h"

class LayerGame : public Layer
{
public:
	LayerGame();
	~LayerGame();

	void Start() override;

	void PreUpdate() override;
	void Update() override;
	void PostUpdate() override;

	void CleanUp() override;

	std::vector<MeshRenderComponent*> meshComponentTest;

public:
	
};

