#pragma once

#include "Mesh.h"
#include "Shader.h"

/// <summary>
/// This class manages the rendering process o a single MeshObject.
/// A MeshObject is a colection of Meshes that follow the same vertices/indices pattern.
/// TLDR: This class instance renders a MeshObject.
/// </summary>
class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	uint SetMeshInformation(Mesh& mesh);

	void Draw();

	uint AddMesh(Mesh& mesh);

	std::map<uint, Mesh>& GetMap() { return meshes; };

public:
	bool initialized = false;
private:
	void CreateBuffers();
	Shader* basicShader = nullptr;

private:
	std::map<uint, Mesh> meshes;
	std::vector<Vertex> totalVertices;
	std::vector<uint> totalIndices;
	std::vector<mat4x4> modelMatrices;

	uint VAO = 0; // Vertex Array
	uint VBO = 0; // Vertex buffer
	uint IBO = 0; // Elements buffer object
	uint MBO = 0; // ModelMatrix buffer object

	int IDcounter = 0;
};

