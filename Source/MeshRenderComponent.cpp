#include "Headers.h"
#include "MeshRenderComponent.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModelRenderManager.h"
#include "RenderManager.h"
#include "Mesh.h"
#include "TextureImporter.h"
#include "ModuleResourceManager.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"

MeshRenderComponent::MeshRenderComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Type::MESH_RENDERER;
	_meshID = -1;
	_instanceID = 0;
	_needsTransformCallback = true;
}

MeshRenderComponent::~MeshRenderComponent()
{
	if (_meshID != -1)
	{
		RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);
		manager->GetMap().erase(_instanceID);
	}
}

void MeshRenderComponent::InitAsLoadedMesh(uint meshID)
{
	_meshID = meshID;

	ModelRenderManager* test = &Application::Instance()->renderer3D->modelRender;

	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);

	Mesh instanceMesh;

	_instanceID = manager->AddMesh(instanceMesh); // TODO: Doesnt need to add an empty mesh. 

	vertexNum = manager->GetMeshVertexNum();
	indexNum = manager->GetMeshIndexNum();
	_gameObject->transform->ForceUpdate();
	GetMesh().component = this;
}

void MeshRenderComponent::InitAsNewMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
	Mesh newMesh;
	newMesh.InitAsMesh(vertices, indices);

	_meshID = Application::Instance()->renderer3D->modelRender.GetMapSize();
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID); // Create a renderManager.

	_instanceID = manager->SetMeshInformation(newMesh);

	vertexNum = manager->GetMeshVertexNum();
	indexNum = manager->GetMeshIndexNum();
	_gameObject->transform->ForceUpdate();
	GetMesh().component = this;
}

void MeshRenderComponent::OnTransformCallback(float4x4 worldMatrix)
{
	Mesh& mesh = GetMesh();
	mesh.modelMatrix = worldMatrix.Transposed();
	mesh.CalculateBoundingBoxes();
}

void MeshRenderComponent::OnEnable()
{
	GetMesh().draw = true;
}

void MeshRenderComponent::OnDisable()
{
	GetMesh().draw = false;
}

Mesh& MeshRenderComponent::GetMesh()
{
	if (!isTransparent)
	{
		RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);
		Mesh& meshReference = manager->GetMap()[_instanceID];
		return meshReference;
	}
	else
	{
		Mesh& meshReference = Application::Instance()->renderer3D->modelRender._transparencyMeshes[_meshID];
		return meshReference;
	}
}

void MeshRenderComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (_meshID == -1)
		{
			ImGui::TextWrapped("No mesh loaded! Drag an FBX file below to load a mesh ");

			ImGui::TextColored(ImVec4(1,1,0,1), "Drag FBX here");

			// Create Droped mesh
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Mesh"))
				{
					const std::string drop = *(std::string*)payload->Data;

					Resource* resource = Application::Instance()->resource->LoadFile(drop);

					if (resource->type != ResourceType::MESH) return;

					ResourceMesh* mesh = (ResourceMesh*)resource;

					GameObject* meshParent = mesh->meshParent;

					if (meshParent == nullptr) return;

					std::vector<GameObject*>* meshes = meshParent->GetChildren();

					for (uint i = 0; i < meshes->size(); i) // Because the vector changes dynamically, we just read the value 0 every iteration.
					{
						meshes->at(i)->SetParent(_gameObject);
					}

					MeshRenderComponent* meshRenderer = meshParent->GetComponent<MeshRenderComponent>();

					//TODO Maybe turn this into a method?

					if (meshRenderer != nullptr)
					{
						_meshID = meshRenderer->_meshID;
						_instanceID = meshRenderer->_instanceID;
						vertexNum = meshRenderer->vertexNum;
						indexNum = meshRenderer->indexNum;
						meshRenderer->_meshID = -1;
						GetMesh().component = this;
					}

					meshParent->Destroy();

					std::string popUpmessage = "Loaded Mesh: " + drop;
					Application::Instance()->layers->editor->AddPopUpMessage(popUpmessage);

				}
				ImGui::EndDragDropTarget();
			}

			return;
		}

		Mesh& mesh = GetMesh();
		bool auxiliaryBool = _isEnabled;
		if (ImGui::Checkbox("Active##Mesh", &auxiliaryBool))
			auxiliaryBool ? Enable() : Disable();

		ImGui::TextWrapped("Mesh vertices: "); ImGui::SameLine(); 
		ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(vertexNum).c_str());

		ImGui::TextWrapped("Mesh indices: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(indexNum).c_str());

		ImGui::Checkbox("Vertex Normals", &mesh.showVertexNormals); ImGui::SameLine();
		ImGui::Checkbox("Face Normals", &mesh.showFaceNormals);

		ImGui::Checkbox("AABB", &mesh.showAABB); ImGui::SameLine();
		ImGui::Checkbox("OBB", &mesh.showOBB);
	}
}

void MeshRenderComponent::SetMeshAsTransparent()
{
	if (isTransparent)
		return;

	GetMesh().CleanUp();

	isTransparent = true;

	// Erase current mesh 
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);
	manager->GetMap().erase(_instanceID);

	// When using a transparent mesh, MeshID is the key in the map!
	_meshID = Application::Instance()->renderer3D->modelRender.AddTransparentMesh(manager, this);

	// Update mesh transfrom 
	Mesh& mesh = GetMesh();
	mesh.modelMatrix = _gameObject->transform->GetGlobalMatrix(true); // Force dirty flag update.
	mesh.CalculateBoundingBoxes();
}

void MeshRenderComponent::SetMeshAsOpaque()
{
	if (!isTransparent)
		return;
}

