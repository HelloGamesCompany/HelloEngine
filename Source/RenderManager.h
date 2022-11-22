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

	int GetMeshVertexNum() { return totalVertices.size(); }
	int GetMeshIndexNum() { return totalIndices.size(); }

public:
	bool initialized = false;
private:
	void CreateBuffers();
	void CreateNormalsDisplayBuffer();
	void CreateAABB();

	void DrawVertexNormals(int modelMatrixIndex);
	void DrawFaceNormals(int modelMatrixIndex);
	void DrawBoundingBoxAABB(Mesh& mesh);
	void DrawBoundingBoxOBB(Mesh& mesh);


private:
	AABB localAABB;

	Shader* instancedShader = nullptr;
	Shader* lineShader = nullptr;
	Shader* localLineShader = nullptr;

	std::map<uint, Mesh> meshes;
	std::vector<Vertex> totalVertices;
	std::vector<uint> totalIndices;
	std::vector<float4x4> modelMatrices;
	std::vector<float> textureIDs;

	std::vector<float3> vertexNormalsDisplay;
	std::vector<float3> faceNormalsDisplay;

	std::vector<uint> boxIndices; // Used to display bounding boxes.

	uint VAO = 0; // Vertex Array
	uint VBO = 0; // Vertex buffer
	uint IBO = 0; // Elements buffer object
	uint MBO = 0; // ModelMatrix buffer object
	uint TBO = 0; // TextureID buffer object 

	uint OBBIndexO = 0; // Elements buffer object for OBB
	uint AABBIndexO = 0; // Elements buffer object for AABB

	uint VertexLineVAO = 0; // Lines to display Vertex Normals
	uint VertexLineVBO = 0;

	uint FaceLineVAO = 0; // Lines to display Face Normals
	uint FaceLineVBO = 0;

	uint OBBLineVAO = 0; // Lines to display OBB
	uint OBBLineVBO = 0;

	uint AABBLineVAO = 0; // Lines to display AABB
	uint AABBLineVBO = 0;

	bool drawVertexNormals = false;
	bool drawFaceNormals = true;

	int IDcounter = 0;

	friend class ModelRenderManager;
	friend class MeshRenderComponent;
};

