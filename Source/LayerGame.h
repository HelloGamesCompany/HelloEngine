#pragma once
#include "Layer.h"
#include "RenderManager.h"

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

	uint GetTextureBuffer() { return textureColorbuffer; }

private:
	uint FBO = 0; // Frame Buffer Object (contains the result of the entire frame texture)
	uint textureColorbuffer = 0;
	uint RBO = 0;

	RenderManager renderManager1;
	RenderManager renderManager2;
};

