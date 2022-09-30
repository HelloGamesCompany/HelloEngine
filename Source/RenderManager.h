#pragma once

#include "Mesh.h"

class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	void Draw();

	void TestOnEditor();

	void AddMesh(Mesh newMesh);

private:
	void CreateBuffers();

private:
	std::vector<Mesh> meshes;
	std::vector<float3> totalVertices;
	std::vector<uint> totalIndices;

	uint VAO = 0; // Vertex Array
	uint VBO = 0; // Vertex buffer
	uint IBO = 0; // Elements buffer object
};

