#pragma once

#include "Mesh.h"

class MeshRenderComponent
{
public:
	MeshRenderComponent();
	~MeshRenderComponent();

	void InitAsDefaultCube(float3 position = { 0.0f,0.0f,0.0f }, float3 scale = { 1.0f,1.0f,1.0f });
	void InitAsDefaultSphere(float3 position = { 0.0f,0.0f,0.0f }, float3 scale = { 1.0f,1.0f,1.0f });
	void InitAsLoadedMesh();

	Mesh& GetMesh();

private:
	uint _modelID;
	uint _meshID;
};

