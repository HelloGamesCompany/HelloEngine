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

private:
	ResourceMesh* resource = nullptr;
	int _meshID = -1;
	uint _instanceID = 0;

	int selectedNormalDisplay = 0;

	int vertexNum = 0;
	int indexNum = 0;

	bool isTransparent = false;

#ifdef STANDALONE
	uint resourceUID = 0;
#endif // STANDALONE


	friend class Mesh;
};