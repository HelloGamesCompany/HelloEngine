#pragma once

#include "Mesh.h"
#include "Component.h"

class MeshRenderComponent : public Component
{
public:
	MeshRenderComponent(GameObject* gameObject);
	virtual ~MeshRenderComponent() override;

	void InitAsDefaultCube(float3 position = { 0.0f,0.0f,0.0f }, float3 scale = { 1.0f,1.0f,1.0f });
	void InitAsDefaultSphere(float3 position = { 0.0f,0.0f,0.0f }, float3 scale = { 1.0f,1.0f,1.0f });
	void InitAsLoadedMesh(uint meshID);
	void InitAsNewMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices);

	Mesh& GetMesh();

private:
	uint _meshID;
	uint _instanceID;
};

