#pragma once

#include "Mesh.h"
#include "Component.h"
#include "ModuleResourceManager.h"

class MeshRenderComponent : public Component
{
public:
	MeshRenderComponent(GameObject* gameObject);
	virtual ~MeshRenderComponent() override;

	void InitAsLoadedMesh(uint meshID);
	void InitAsNewMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices);

	void CreateMesh(uint resourceUID);

	void OnTransformCallback(float4x4 worldMatrix) override;

	void OnEnable() override;
	void OnDisable() override;

	Mesh& GetMesh();

	std::vector<Vertex>& GetMeshVertices();
	std::vector<uint>& GetMeshIndices();

	void OnEditor() override;

	void SetMeshAsTransparent();
	void SetMeshAsOpaque();

	void MarkAsDead() override;
	void MarkAsAlive() override;

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;
private:
	ResourceMesh* _resource = nullptr;
	int64_t _meshID = -1;
	uint _instanceID = 0;

	int _selectedNormalDisplay = 0;

	int _vertexNum = 0;
	int _indexNum = 0;

	bool _isTransparent = false;

#ifdef STANDALONE
	uint _resourceUID = 0;
#endif // STANDALONE

	friend class Mesh;
};