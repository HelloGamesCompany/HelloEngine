#include "Headers.h"
#include "ModelRenderManager.h"
#include "ModuleLayers.h"
#include "MeshRenderComponent.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "MeshImporter.h"
#include "CameraObject.h"
#include "ModuleCamera3D.h"

ModelRenderManager::ModelRenderManager()
{
}

ModelRenderManager::~ModelRenderManager()
{
	RELEASE(_textureManager);
}

void ModelRenderManager::Init()
{
	_textureManager = new TextureManager();
}

RenderManager* ModelRenderManager::GetRenderManager(uint ID)
{
	return &_renderMap[ID];
}

void ModelRenderManager::Draw()
{
	// Draw opaque meshes instanced.
	for (auto& obj : _renderMap)
	{
		obj.second.Draw();
	}

	CameraObject* currentCamera = Application::Instance()->camera->currentDrawingCamera;

	// Draw transparent objects with a draw call per mesh.
	for (auto& mesh : _transparencyMeshes)
	{
		float3 cameraPos = currentCamera->cameraFrustum.pos;
		float distance = mesh.second.modelMatrix.Transposed().TranslatePart().DistanceSq(currentCamera->cameraFrustum.pos);
		_orderedMeshes.emplace(std::make_pair(distance, &mesh.second));
	}

	// iterate meshes from furthest to closest.
	for (auto mesh = _orderedMeshes.rbegin(); mesh != _orderedMeshes.rend(); mesh++)
	{
		// Do camera culling checks first
		if (currentCamera->isCullingActive)
		{
			if (!currentCamera->IsInsideFrustum(mesh->second->globalAABB))
			{
				mesh->second->outOfFrustum = true;
				continue;
			}
			else
				mesh->second->outOfFrustum = false;
		}
		else if (currentCamera->type != CameraType::SCENE)
		{
			mesh->second->outOfFrustum = false;
		}

		// Update mesh. If the mesh should draw this frame, call Draw.
		if (mesh->second->Update())
		{
			mesh->second->Draw();
		}
	}

	_orderedMeshes.clear();
}

uint ModelRenderManager::AddTransparentMesh(RenderManager* previousRenderer, MeshRenderComponent* component)
{
	uint randomID = HelloUUID::GenerateUUID();

	_transparencyMeshes[randomID].InitAsMesh(previousRenderer->totalVertices, previousRenderer->totalIndices);
	_transparencyMeshes[randomID].localAABB = previousRenderer->localAABB;
	_transparencyMeshes[randomID].isTransparent = true;
	_transparencyMeshes[randomID].CreateBufferData();

	return randomID;
}

void ModelRenderManager::CreatePrimitive(GameObject* parent, PrimitiveType type)
{
	if (parent == nullptr) parent = Application::Instance()->layers->rootGameObject;

	switch (type)
	{
		case PrimitiveType::CUBE:
		{
			GameObject* cube = MeshImporter::LoadModel("Resources/Models/3803316393.hmodel");
			cube->SetParent(parent);
			cube->name = "Cube";
			break;
		}
		case PrimitiveType::SPHERE:
		{
			GameObject* sphere = MeshImporter::LoadModel("Resources/Models/363196802.hmodel");
			sphere->SetParent(parent);
			sphere->name = "Sphere";
			break;
		}
		case PrimitiveType::CYLINDER:
		{
			GameObject* cylinder = MeshImporter::LoadModel("Resources/Models/343324178.hmodel");
			cylinder->SetParent(parent);
			cylinder->name = "Cylinder";
			break;
		}
		case PrimitiveType::PLANE:
		{
			GameObject* plane = MeshImporter::LoadModel("Resources/Models/505199352.hmodel");
			plane->SetParent(parent);
			plane->name = "Plane";
			break;
		}
	}
}
