#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "ModuleResourceManager.h"

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

	uint SetMeshInformation(ResourceMesh* resource);

	void Draw();

	uint AddMesh(Mesh& mesh);

	std::map<uint, Mesh>& GetMap() { return meshes; };

	int GetMeshVertexNum() { return totalVertices->size(); }
	int GetMeshIndexNum() { return totalIndices->size(); }

	/// Draws an instance with an individual draw call.
	void DrawInstance(Mesh* mesh, bool useBasicShader = true);

public:
	bool initialized = false;
	ResourceMesh* resource = nullptr;
private:
	void CreateBuffers();
	void CreateBasicBuffers(); // Creates buffers for individual drawing.
	void CreateNormalsDisplayBuffer();
	void CreateAABB();

	void DrawVertexNormals(Mesh& mesh);
	void DrawFaceNormals(Mesh& mesh);
	void DrawBoundingBoxAABB(Mesh& mesh);
	void DrawBoundingBoxOBB(Mesh& mesh);

	void ReallocateMoreMemory();
	void DestroyDynamicBuffers();
	void CreateDynamicBuffers();

private:
	AABB localAABB;

	Shader* instancedShader = nullptr;
	Shader* lineShader = nullptr;
	Shader* localLineShader = nullptr;
	Shader* perMeshShader = nullptr;

	std::map<uint, Mesh> meshes;
	std::vector<Vertex>* totalVertices = nullptr;
	std::vector<uint>* totalIndices = nullptr;
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

	// Buffers to be able to draw a single instance with an individual draw call.
	uint BasicVAO = 0;
	uint BasicVBO = 0;
	uint BasicIBO = 0;

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

	int instanceNum = 10; // Number of instances available in this RenderManager.

	int IDcounter = 0;

	Mesh* _selectedMesh = nullptr;

	friend class ModelRenderManager;
	friend class MeshRenderComponent;
};

