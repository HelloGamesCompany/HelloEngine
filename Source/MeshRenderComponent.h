#pragma once

#include "Mesh.h"
#include "Component.h"

class MeshRenderComponent : public Component
{
public:
	MeshRenderComponent(GameObject* gameObject);
	virtual ~MeshRenderComponent() override;

	void InitAsLoadedMesh(uint meshID);
	void InitAsNewMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices);

	void OnTransformCallback(float4x4 worldMatrix) override;

	void OnEnable() override;
	void OnDisable() override;

	Mesh& GetMesh();

	void OnEditor() override;

	void SetMeshAsTransparent();
	void SetMeshAsOpaque();

private:
	int _meshID = -1;
	uint _instanceID = 0;

	std::string comboValues[3] = { "None", "Vertex Normals", "Face Normals" };
	int selectedNormalDisplay = 0;

	int vertexNum = 0;
	int indexNum = 0;

	bool isTransparent = false;

	friend class Mesh;
};