#pragma once
#include "Component.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshRenderComponent.h"

class MaterialComponent : public Component
{
public:

	MaterialComponent(GameObject* go);
	~MaterialComponent();

	void OnEditor() override;

	void OnEnable() override;
	void OnDisable() override;

	void SetMeshRenderer(MeshRenderComponent* mesh);

	Mesh& GetMesh();

private:
	MeshRenderComponent* meshRenderer = nullptr;

private:
	void ChangeTexture(uint textureID);
	int textureID = -1;
};

