#pragma once

#include "Mesh.h"
#include "Component.h"
#include "ModuleResourceManager.h"

enum class MeshRenderType
{
	INSTANCED,
	INDEPENDENT,
	TRANSPARENCY
};

class MeshRenderComponent : public Component
{
public:
	MeshRenderComponent(GameObject* gameObject);
	MeshRenderComponent(GameObject* gameObject, const MeshRenderComponent& copy);
	virtual ~MeshRenderComponent() override;

	void CreateMesh(uint resourceUID, MeshRenderType type = MeshRenderType::INSTANCED);

	void OnTransformCallback(float4x4 worldMatrix) override;

	void OnEnable() override;
	void OnDisable() override;

	Mesh& GetMesh();

	void DestroyMesh();

	std::vector<Vertex>& GetMeshVertices();
	std::vector<uint>& GetMeshIndices();

	/// Changes mesh render type to Transparency, Independent or Instance. 
	void ChangeMeshRenderType(MeshRenderType type);

#ifdef STANDALONE
	void OnEditor() override;

	void MarkAsDead() override;
	void MarkAsAlive() override;
#endif
	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	uint GetResourceUID();
	void DestroyedResource();

	void UnlinkResource();

private:
	ResourceMesh* _resource = nullptr;
	int64_t _meshID = -1;
	uint _instanceID = 0;

	int _selectedNormalDisplay = 0;

	int _vertexNum = 0;
	int _indexNum = 0;

	bool _isTransparent = false;
	MeshRenderType renderType = MeshRenderType::INSTANCED;

	// Temporal. Delete later:
	std::string _comboOptions[3] = { "INSTANCE", "INDEPENDENT", "TRANSPARENCY" };
	uint currentCombo = 0;
	uint _resourceUID = 0;

	friend class Mesh;
};