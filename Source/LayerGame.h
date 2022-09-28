#pragma once
#include "Layer.h"
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

	void VertexBufferArraySetUp();

private:
	uint VAO = 0;
	uint VBO = 0;	
	uint IBO = 0;
};

