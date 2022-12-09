#include "Headers.h"
#include "RenderManager.h"
#include "ModuleLayers.h"
#include "MeshRenderComponent.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "MeshImporter.h"
#include "CameraObject.h"
#include "ModuleCamera3D.h"

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
	RELEASE(_textureManager);
}

void RenderManager::Init()
{
	_textureManager = new TextureManager();

	cubeUID = 2677981019;
	planeUID = 2393626792;
	cylinderUID = 1158218481;
	sphereUID = 2121897186;

	// Create resources for 2121897186
	ModuleResourceManager::S_CreateResourceMesh("Resources/Editor/Primitives/2677981019.hmesh", cubeUID, "Cube", false);
	ModuleResourceManager::S_CreateResourceMesh("Resources/Editor/Primitives/2393626792.hmesh", planeUID, "Plane", false);
	ModuleResourceManager::S_CreateResourceMesh("Resources/Editor/Primitives/1158218481.hmesh", cylinderUID, "Cylinder", false);
	ModuleResourceManager::S_CreateResourceMesh("Resources/Editor/Primitives/2121897186.hmesh", sphereUID, "Sphere", false);

}

void RenderManager::OnEditor()
{
	if (ImGui::Begin("Render System Debugging"))
	{
		ImGui::TextWrapped("Window to manage every RenderManager memory usage and instances. The memory allocated per RenderManager increases by 50 per cent every time the current allocated memory gets surpassed.");
		for (auto& manager : _renderMap)
		{
			if (manager.second.resource == nullptr)
				continue;
			std::string headerName = manager.second.resource->debugName + "##" + std::to_string(manager.second.resource->UID);
			if(ImGui::CollapsingHeader(headerName.c_str()))
			{
				std::string maxInstances = "Maximum number of instances: " + std::to_string(manager.second.instanceNum);
				ImGui::Text(maxInstances.c_str());
				ImGui::Text("Instances: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), std::to_string(manager.second.meshes.size()).c_str());
				ImGui::Text("Memory usage in bytes: "); ImGui::SameLine();
				float memory = manager.second.instanceNum * sizeof(float4x4) + manager.second.instanceNum * sizeof(float);
				memory += manager.second.totalIndices->size() * sizeof(uint);
				memory += manager.second.totalVertices->size() * sizeof(Vertex);
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), std::to_string(memory).c_str());
			}
		}
	}
	ImGui::End();
}

InstanceRenderer* RenderManager::GetRenderManager(uint ID)
{
	// If there is no instance Renderer for this mesh resource
	if (_renderMap.count(ID) == 0)
	{
		_renderMap[ID].SetMeshInformation((ResourceMesh*)ModuleResourceManager::resources[ID]);
	}

	return &_renderMap[ID];
}

void RenderManager::Draw()
{
	// Draw opaque meshes instanced.
	for (auto& obj : _renderMap)
	{
		obj.second.Draw();
	}
	// Delete empty render managers.
	for (int i = 0; i < _emptyRenderManagers.size(); i++)
	{
		_renderMap.erase(_emptyRenderManagers[i]);
	}
	_emptyRenderManagers.clear();
	
	// Draw meshes that must be rendered in an individual draw call.

	// Draw selected mesh
	DrawSelectedMesh();

	// Draw meshes that have transparency textures applied on their material.
	DrawTransparentMeshes();
}

uint RenderManager::AddTransparentMesh(ResourceMesh* resource)
{
	uint randomID = HelloUUID::GenerateUUID();

	_transparencyMeshes[randomID].InitWithResource(resource);
	_transparencyMeshes[randomID].localAABB = resource->localAABB;
	_transparencyMeshes[randomID].isTransparent = true;
	_transparencyMeshes[randomID].CreateBufferData();

	return randomID;
}

void RenderManager::AddIndependentMesh()
{
}

void RenderManager::CreatePrimitive(GameObject* parent, PrimitiveType type)
{
	if (parent == nullptr) parent = Application::Instance()->layers->rootGameObject;

	switch (type)
	{
		case PrimitiveType::CUBE:
		{
			GameObject* cube = new GameObject(parent, "Cube", "Primitive");
			MeshRenderComponent* meshRenderer = cube->AddComponent<MeshRenderComponent>();
			meshRenderer->CreateMesh(cubeUID);
			break;
		}
		case PrimitiveType::SPHERE:
		{
			GameObject* cube = new GameObject(parent, "Sphere", "Primitive");
			MeshRenderComponent* meshRenderer = cube->AddComponent<MeshRenderComponent>();
			meshRenderer->CreateMesh(sphereUID);
			break;
		}
		case PrimitiveType::CYLINDER:
		{
			GameObject* cube = new GameObject(parent, "Cylinder", "Primitive");
			MeshRenderComponent* meshRenderer = cube->AddComponent<MeshRenderComponent>();
			meshRenderer->CreateMesh(cylinderUID);
			break;
		}
		case PrimitiveType::PLANE:
		{
			GameObject* cube = new GameObject(parent, "Plane", "Primitive");
			MeshRenderComponent* meshRenderer = cube->AddComponent<MeshRenderComponent>();
			meshRenderer->CreateMesh(planeUID);
			break;
		}
	}
}

void RenderManager::DestroyRenderManager(uint managerUID)
{
	_emptyRenderManagers.push_back(managerUID);
}

void RenderManager::SetSelectedMesh(Mesh* mesh)
{
	selectedMesh = mesh;
}

void RenderManager::DrawSelectedMesh()
{
	if (selectedMesh == nullptr)
		return;

	selectedMesh->DrawAsSelected();

	selectedMesh = nullptr;
}

void RenderManager::DrawTransparentMeshes()
{
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
