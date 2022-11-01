#include "Headers.h"
#include "ModelRenderManager.h"
#include "ModuleLayers.h"
#include "MeshRenderComponent.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "MeshImporter.h"

ModelRenderManager::ModelRenderManager()
{
}

ModelRenderManager::~ModelRenderManager()
{
	RELEASE(textureManager);
}

void ModelRenderManager::Init()
{
	textureManager = new TextureManager();
}

RenderManager* ModelRenderManager::GetRenderManager(uint ID)
{
	return &renderMap[ID];
}

void ModelRenderManager::Draw()
{
	for (auto& obj : renderMap)
	{
		obj.second.Draw();
	}
}

void ModelRenderManager::CreatePrimitive(GameObject* parent, PrimitiveType type)
{
	if (parent == nullptr) parent = Application::Instance()->layers->rootGameObject;

	switch (type)
	{
		case PrimitiveType::CUBE:
		{
			GameObject* cube = MeshImporter::LoadMesh("Assets/Primitives/cube.fbx");
			cube->SetParent(parent);
			cube->name = "Cube";
			break;
		}
		case PrimitiveType::SPHERE:
		{
			GameObject* sphere = MeshImporter::LoadMesh("Assets/Primitives/sphere.fbx");
			sphere->SetParent(parent);
			sphere->name = "Sphere";
			break;
		}
		case PrimitiveType::CYLINDER:
		{
			GameObject* cylinder = MeshImporter::LoadMesh("Assets/Primitives/cylinder.fbx");
			cylinder->SetParent(parent);
			cylinder->name = "Cylinder";
			break;
		}
		case PrimitiveType::PLANE:
		{
			GameObject* plane = MeshImporter::LoadMesh("Assets/Primitives/plane.fbx");
			plane->SetParent(parent);
			plane->name = "Plane";
			break;
		}
	}
}
