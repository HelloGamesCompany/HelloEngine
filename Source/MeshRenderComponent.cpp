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
		if (!isTransparent)
		{
			RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);
			manager->GetMap().erase(_instanceID);
			_meshID = -1;
		}
		else
		{
			Application::Instance()->renderer3D->modelRender._transparencyMeshes.erase(_meshID);
		}
		if (resource != nullptr)
			resource->Dereference();
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

	//_instanceID = manager->SetMeshInformation(newMesh);

	vertexNum = manager->GetMeshVertexNum();
	indexNum = manager->GetMeshIndexNum();
	_gameObject->transform->ForceUpdate();
	GetMesh().component = this;
}

void MeshRenderComponent::CreateMesh(uint resourceUID)
{
	resource = (ResourceMesh*)ModuleResourceManager::S_LoadResource(resourceUID);
	
	if (resource == nullptr)
	{
		_meshID = -1;
		return;
	}
	
	_meshID = resourceUID;

	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID); // Create a renderManager.
	
	if (manager->initialized)
	{
		Mesh instanceMesh;
		_instanceID = manager->AddMesh(instanceMesh);
	}
	else
	{
		_instanceID = manager->SetMeshInformation(resource);
	}

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

std::vector<Vertex>& MeshRenderComponent::GetMeshVertices()
{
	if (!isTransparent)
	{
		RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);
		return *manager->totalVertices;
	}
	else
	{
		return *GetMesh()._vertices;
	}
}

std::vector<uint>& MeshRenderComponent::GetMeshIndices()
{
	if (!isTransparent)
	{
		RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);
		return *manager->totalIndices;
	}
	else
	{
		return *GetMesh()._indices;
	}
}

void MeshRenderComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (_meshID == -1)
		{
			ImGui::TextWrapped("No mesh loaded! Drag an .hmesh file below to load a mesh ");

			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Drag .hmesh here"); ImGui::SameLine();

			// Create Droped mesh
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Mesh"))
				{
					const uint* drop = (uint*)payload->Data;

					CreateMesh(*drop);

					std::string popUpmessage = "Loaded Mesh: ";
					Application::Instance()->layers->editor->AddPopUpMessage(popUpmessage);

				}
				ImGui::EndDragDropTarget();
			}
			ImGui::HelpMarker("You can find .hmesh files by clicking on any model file (FBX or DAE). They will appear below the file icon in the Project window.");

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
	_meshID = Application::Instance()->renderer3D->modelRender.AddTransparentMesh(manager, resource);

	// Update mesh transfrom 
	Mesh& mesh = GetMesh();
	mesh.component = this;
	mesh.modelMatrix = _gameObject->transform->GetGlobalMatrix(true).Transposed(); // Force dirty flag update.
	mesh.CalculateBoundingBoxes();
}

void MeshRenderComponent::SetMeshAsOpaque()
{
	if (!isTransparent)
		return;
	// This will work using Resources. We need to know what resource do we belong to. If that resource is not loaded, load it and create a Render Manager.
	// if the Resource is loaded, then a Render Manaer that uses that resource should be already created.
	// Use the correct Render Manager and Add a mesh instance to it.
}

void MeshRenderComponent::MarkAsDead()
{
	if (_meshID != -1)
	{
		if (!isTransparent)
		{
			RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);
			manager->GetMap().erase(_instanceID);
			_meshID = -1;
		}
		else
		{
			Application::Instance()->renderer3D->modelRender._transparencyMeshes.erase(_meshID);
		}
		if (resource != nullptr)
		{
			resource->Dereference();
			resourceUID = resource->UID;
			resource = nullptr;
		}
	}
}

void MeshRenderComponent::MarkAsAlive()
{
	CreateMesh(resourceUID);
}

void MeshRenderComponent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["ResourceUID"] = resource ? resource->UID : 0;
	j["Components"].push_back(_j);
}

void MeshRenderComponent::DeSerialization(json& j)
{
	CreateMesh(j["ResourceUID"]);
}