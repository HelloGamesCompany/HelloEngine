#pragma once
#include "Component.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshRenderComponent.h"
#include "ModuleResourceManager.h"

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

	void ChangeTexture(ResourceTexture* resource);
	void ChangeTexture(int ID);

private:
	MeshRenderComponent* meshRenderer = nullptr;
	ResourceTexture* currentResource = nullptr;

	int textureID = -1;
};

