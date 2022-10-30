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

	void SetMeshRenderer(MeshRenderComponent* mesh);

	Mesh& GetMesh();

	void Enable(bool enabled, bool fromGo = true) override;

private:
	MeshRenderComponent* meshRenderer = nullptr;

private:
	void ChangeTexture(uint textureID);
	int textureID = -1;
};

