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
		if (!_isTransparent)
		{
			RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);
			manager->GetMap().erase(_instanceID);
			_meshID = -1;
		}
		else
		{
			Application::Instance()->renderer3D->modelRender._transparencyMeshes.erase(_meshID);
		}
		if (_resource != nullptr)
			_resource->Dereference();
	}
}

void MeshRenderComponent::InitAsLoadedMesh(uint meshID)
{
	_meshID = meshID;

	ModelRenderManager* test = &Application::Instance()->renderer3D->modelRender;

	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);

	Mesh instanceMesh;

	_instanceID = manager->AddMesh(instanceMesh); // TODO: Doesnt need to add an empty mesh. 

	_vertexNum = manager->GetMeshVertexNum();
	_indexNum = manager->GetMeshIndexNum();
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

	_vertexNum = manager->GetMeshVertexNum();
	_indexNum = manager->GetMeshIndexNum();
	_gameObject->transform->ForceUpdate();
	GetMesh().component = this;
}

void MeshRenderComponent::CreateMesh(uint resourceUID)
{
	_resource = (ResourceMesh*)ModuleResourceManager::S_LoadResource(resourceUID);
	
	if (_resource == nullptr)
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
		_instanceID = manager->SetMeshInformation(_resource);
	}

	_vertexNum = manager->GetMeshVertexNum();
	_indexNum = manager->GetMeshIndexNum();
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
	if (!_isTransparent)
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
	if (!_isTransparent)
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
	if (!_isTransparent)
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
		ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(_vertexNum).c_str());

		ImGui::TextWrapped("Mesh indices: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(_indexNum).c_str());

		ImGui::Checkbox("Vertex Normals", &mesh.showVertexNormals); ImGui::SameLine();
		ImGui::Checkbox("Face Normals", &mesh.showFaceNormals);

		ImGui::Checkbox("AABB", &mesh.showAABB); ImGui::SameLine();
		ImGui::Checkbox("OBB", &mesh.showOBB);
	}
}

void MeshRenderComponent::SetMeshAsTransparent()
{
	if (_isTransparent)
		return;

	GetMesh().CleanUp();

	_isTransparent = true;

	// Erase current mesh 
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);
	manager->GetMap().erase(_instanceID);

	// When using a transparent mesh, MeshID is the key in the map!
	_meshID = Application::Instance()->renderer3D->modelRender.AddTransparentMesh(manager, _resource);

	// Update mesh transfrom 
	Mesh& mesh = GetMesh();
	mesh.component = this;
	mesh.modelMatrix = _gameObject->transform->GetGlobalMatrix(true).Transposed(); // Force dirty flag update.
	mesh.CalculateBoundingBoxes();
}

void MeshRenderComponent::SetMeshAsOpaque()
{
	if (!_isTransparent)
		return;
	// This will work using Resources. We need to know what resource do we belong to. If that resource is not loaded, load it and create a Render Manager.
	// if the Resource is loaded, then a Render Manaer that uses that resource should be already created.
	// Use the correct Render Manager and Add a mesh instance to it.
}

void MeshRenderComponent::MarkAsDead()
{
	if (_meshID != -1)
	{
		if (!_isTransparent)
		{
			RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);
			manager->GetMap().erase(_instanceID);
			_meshID = -1;
		}
		else
		{
			Application::Instance()->renderer3D->modelRender._transparencyMeshes.erase(_meshID);
		}
		if (_resource != nullptr)
		{
			_resource->Dereference();
			_resourceUID = _resource->UID;
			_resource = nullptr;
		}
	}
}

void MeshRenderComponent::MarkAsAlive()
{
	CreateMesh(_resourceUID);
}

void MeshRenderComponent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;

	if (_meshID != -1)
	{
		Mesh& m = GetMesh();
		_j["ShowAABB"] = m.showAABB;
		_j["ShowOBB"] = m.showOBB;
		_j["ShowFaceNormals"] = m.showFaceNormals;
		_j["ShowVertexNormals"] = m.showVertexNormals;
	}
	else
	{
		_j["ShowAABB"] = false;
		_j["ShowOBB"] = false;
		_j["ShowFaceNormals"] = false;
		_j["ShowVertexNormals"] = false;
	}

	if (_resource != nullptr)
	{
		_j["ModelUID"] = _resource->modelUID;
		_j["Index inside model"] = _resource->indexInsideModel;
	}
	else
	{
		_j["ModelUID"] = 0;
		_j["Index inside model"] = 0;
	}

	j["Components"].push_back(_j);
}

void MeshRenderComponent::DeSerialization(json& j)
{
	ResourceModel* model = (ResourceModel*)ModuleResourceManager::resources[j["ModelUID"]];

	if (model != nullptr)
	{
		uint index = j["Index inside model"];
		if (index < model->modelMeshes.size())
		{
			ResourceMesh* resourceMesh = model->modelMeshes[index];
			CreateMesh(resourceMesh->UID);

			if (_meshID != -1)
			{
				Mesh& m = GetMesh();

				m.showAABB = j["ShowAABB"];
				m.showOBB = j["ShowOBB"];
				m.showFaceNormals = j["ShowFaceNormals"];
				m.showVertexNormals = j["ShowVertexNormals"];
			}

			return;
		}
	}
	Console::S_Log("A scene mesh render data was not found.");
}