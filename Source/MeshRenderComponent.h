#pragma once

#include "Mesh.h"

class MeshRenderComponent
{
public:
	MeshRenderComponent();
	~MeshRenderComponent();

	void InitAsDefaultCube();
	void InitAsDefaultSphere();
	void InitAsLoadedMesh();

	Mesh& GetMesh();

private:
	uint modelID;
	uint meshID;
};

