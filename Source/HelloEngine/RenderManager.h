#pragma once

#include "InstanceRenderer.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "MeshRenderComponent.h"
#include "PhysBody3D.h"

enum class PrimitiveType
{
	CUBE,
	SPHERE,
	CYLINDER,
	PLANE,
	PLANE2D, // Used in UI
};

enum class PrimitiveModelsUID
{
	CUBE = 10,
	SPHERE = 11,
	CYLINDER = 12,
	PLANE = 13,
	PLANE2D = 14,
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

	InstanceRenderer* GetRenderManager(uint ID, bool create = true);
	uint GetMapSize() { return _renderMap.size(); };

	void Draw();
	void Draw2D();

	uint AddMesh(ResourceMesh* resource, MeshRenderType type);

	uint AddTransparentMesh(ResourceMesh* resource);
	uint AddIndependentMesh(ResourceMesh* resource);
	uint AddInstancedMesh(ResourceMesh* resource);
	uint Add2DMesh();

	void CreatePrimitive(GameObject* parent, PrimitiveType type);

	void DestroyRenderManager(uint managerUID);

	void SetSelectedMesh(Mesh* mesh);
	void DrawSelectedMesh();

	void DrawVertexNormals(Mesh* mesh);
	void DrawFaceNormals(Mesh* mesh);
	void DrawOBB(Mesh* mesh);
	void DrawAABB(Mesh* mesh);
	void DrawColliderBox(PhysBody3D* mesh);
	void DrawColliderSphere(PhysBody3D* mesh);
	void DrawColliderCylinder(PhysBody3D* mesh);

	void DestroyInstanceRenderers();

private:
	void DrawTransparentMeshes();
	void DrawIndependentMeshes();

private:
	std::map<uint, InstanceRenderer> _renderMap; // Render managers that use instance rendering to draw opaque meshes.
	std::map<uint, Mesh> _transparencyMeshes; // Meshes with transparency that must be drawn with a draw call per mesh.
	std::multimap<float, Mesh*> _orderedMeshes; // Meshes with transparency ordered from furthest to closest to the camera.
	
	std::map<uint, Mesh> _independentMeshes; // Opaque meshes that need to be drawn in an independent draw call.

	TextureManager* _textureManager = nullptr;
	
	std::vector<uint> _emptyRenderManagers;

	Mesh* _selectedMesh = nullptr;

	std::vector<uint> boxIndices; // Used to display bounding boxes.
	std::vector<uint> sphereIndices;

	const uint sphereVerticalSlices = 16;
	const uint sphereHorizontalSlices = 16;
	const uint sphereVertexNum = sphereVerticalSlices * sphereHorizontalSlices + 2;

	// ModelResources for primitives
	ResourceModel* primitiveModels[5];

	// Shaders for drawing debug information
	Shader* lineShader = nullptr;
	Shader* localLineShader = nullptr;

	uint AABBVAO = 0;
	uint AABBVBO = 0;
	uint OBBVAO = 0;
	uint OBBVBO = 0;
	uint SPVAO = 0;
	uint SPVBO = 0;

	uint AABBIBO = 0; // index buffer object shared by both OBB and ABB buffers above.
	uint OBBIBO = 0;
	uint SPIBO = 0;

	// Primitives
	uint cubeUID = 0;
	uint sphereUID = 0;
	uint planeUID = 0;
	uint cylinderUID = 0;
	uint plane2DUID = 0;

	InstanceRenderer* renderer2D = nullptr;

	friend class MeshRenderComponent;
	friend class ResourceMesh;
	friend class Mesh;
};

