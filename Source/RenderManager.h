#pragma once

#include "InstanceRenderer.h"
#include "TextureManager.h"
#include "GameObject.h"

enum class PrimitiveType
{
	CUBE,
	SPHERE,
	CYLINDER,
	PLANE,
};

/// <summary>
/// This class contains a colletion of RenderManagers. It dynamically creates and destroys Render Managers tu fullfill the task of having one per Unique mesh.
/// Every Render Manager updates and draws their corresponding Models.
/// </summary>
class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	void Init();

	void OnEditor();

	InstanceRenderer* GetRenderManager(uint ID);
	uint GetMapSize() { return _renderMap.size(); };

	void Draw();

	uint AddTransparentMesh(ResourceMesh* resource);

	void AddIndependentMesh();

	void CreatePrimitive(GameObject* parent, PrimitiveType type);

	void DestroyRenderManager(uint managerUID);

	void SetSelectedMesh(Mesh* mesh);
	void DrawSelectedMesh();

	void DrawVertexNormals(Mesh* mesh);
	void DrawFaceNormals(Mesh* mesh);
	void DrawOBB(Mesh* mesh);
	void DrawAABB(Mesh* mesh);

private:
	void DrawTransparentMeshes();

private:
	std::map<uint, InstanceRenderer> _renderMap; // Render managers that use instance rendering to draw opaque meshes.
	std::map<uint, Mesh> _transparencyMeshes; // Meshes with transparency that must be drawn with a draw call per mesh.
	std::multimap<float, Mesh*> _orderedMeshes; // Meshes with transparency ordered from furthest to closest to the camera.
	
	std::map<uint, Mesh> _individualMeshes; // Opaque meshes that need to be drawn in an independent draw call.

	TextureManager* _textureManager = nullptr;
	
	std::vector<uint> _emptyRenderManagers;

	Mesh* selectedMesh = nullptr;

	std::vector<uint> boxIndices; // Used to display bounding boxes.

	// Shaders for drawing debug information
	Shader* lineShader = nullptr;
	Shader* localLineShader = nullptr;

	uint AABBVAO = 0;
	uint AABBVBO = 0;
	uint OBBVAO = 0;
	uint OBBVBO = 0;

	uint AABBIBO = 0; // index buffer object shared by both OBB and ABB buffers above.
	uint OBBIBO = 0;

	// Primitives
	uint cubeUID = 0;
	uint sphereUID = 0;
	uint planeUID = 0;
	uint cylinderUID = 0;

	friend class MeshRenderComponent;
};

