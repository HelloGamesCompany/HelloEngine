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

	uint GetTextureBuffer() { return textureColorbuffer; }

private:
	uint VAO = 0; // Vertex Array Object (contains multiple buffers)
	uint VBO = 0; // Vertex Buffer Object (contains vertex position and other data)
	uint IBO = 0; // Index/Element buffer object (contains indices for a vertex buffer object)
	uint FBO = 0; // Frame Buffer Object (contains the result of the entire frame texture)

	uint textureColorbuffer = 0;
	uint RBO = 0;
};

