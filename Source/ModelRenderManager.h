#pragma once

#include "RenderManager.h"
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
class ModelRenderManager
{
public:
	ModelRenderManager();
	~ModelRenderManager();

	void Init();

	RenderManager* GetRenderManager(uint ID);
	uint GetMapSize() { return _renderMap.size(); };

	void Draw();

	uint AddTransparentMesh(RenderManager* previousRenderer, MeshRenderComponent* component);
	void CreatePrimitive(GameObject* parent, PrimitiveType type);

private:
	std::map<uint, RenderManager> _renderMap; // Render managers that use instance rendering to draw opaque meshes.
	std::map<uint, Mesh> _transparencyMeshes; // Meshes with transparency that must be drawn with a draw call per mesh.
	std::map<float, Mesh*> _orderedMeshes; // Meshes with transparency ordered from furthest to closest to the camera.
	TextureManager* _textureManager = nullptr;

	friend class MeshRenderComponent;
};

