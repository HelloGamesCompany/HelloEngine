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

	void OnPositionUpdate(float3 pos) override;
	void OnRotationUpdate(float3 rot) override;
	void OnScaleUpdate(float3 scale) override;
	void OnTransformUpdate(float3 pos, float3 scale, float3 rot) override;

	Mesh& GetMesh();

	void OnEditor() override;

	void Enable(bool enabled, bool fromGo = true) override;

private:
	uint _meshID = 0;
	uint _instanceID = 0;

	std::string comboValues[3] = { "None", "Vertex Normals", "Face Normals" };
	int selectedNormalDisplay = 0;

	int vertexNum = 0;
	int indexNum = 0;
};