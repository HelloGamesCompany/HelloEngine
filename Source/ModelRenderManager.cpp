#include "Headers.h"
#include "ModelRenderManager.h"
#include "ModuleLayers.h"
#include "MeshRenderComponent.h"
#include "IL/il.h"
#include "IL/ilut.h"

ModelRenderManager::ModelRenderManager()
{
}

ModelRenderManager::~ModelRenderManager()
{
	RELEASE(textureManager);
}

void ModelRenderManager::Init()
{
	ilInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
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
			GameObject* cube = new GameObject(parent, "Cube", "Primitive");
			cube->AddComponent<MeshRenderComponent>()->InitAsDefaultCube();
			break;
		}
		case PrimitiveType::SPHERE:
		{
			GameObject* sphere = new GameObject(parent, "Sphere", "Primitive");
			sphere->AddComponent<MeshRenderComponent>()->InitAsDefaultSphere();
			break;
		}
	}
}
