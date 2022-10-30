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
	instanceMesh.InitAsMeshInformation({ 0.0f,0.0,0.0f }, {1.0f,1.0f,1.0f});

	_instanceID = manager->AddMesh(instanceMesh);

	vertexNum = manager->GetMeshVertexNum();
	indexNum = manager->GetMeshIndexNum();
	_gameObject->transform->ForceUpdate();
}

void MeshRenderComponent::InitAsNewMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
	Mesh newMesh;
	newMesh.InitAsMesh(vertices, indices, { 1.0f,1.0f,1.0f }, { 1.0f,1.0f,1.0f });

	_meshID = Application::Instance()->renderer3D->modelRender.GetMapSize();
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID); // Create a renderManager.

	_instanceID = manager->SetMeshInformation(newMesh);

	vertexNum = manager->GetMeshVertexNum();
	indexNum = manager->GetMeshIndexNum();
	_gameObject->transform->ForceUpdate();
}

void MeshRenderComponent::OnPositionUpdate(float3 pos)
{
	GetMesh().SetPosition(pos);
}

void MeshRenderComponent::OnRotationUpdate(float3 rot)
{
	GetMesh().SetRotation(rot);
}

void MeshRenderComponent::OnScaleUpdate(float3 scale)
{
	GetMesh().SetScale(scale);
}

void MeshRenderComponent::OnTransformUpdate(float3 pos, float3 scale, float3 rot)
{
	GetMesh().SetTransform(pos, scale, rot);
}

Mesh& MeshRenderComponent::GetMesh()
{
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);

	Mesh& meshReference = manager->GetMap()[_instanceID];

	return meshReference;
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
					std::vector<GameObject*>* meshes = meshParent->GetChildren();

					for (int i = 0; i < meshes->size(); i) // Because the vector changes dynamically, we just read the value 0 every iteration.
					{
						meshes->at(i)->SetParent(_gameObject);
					}

					MeshRenderComponent* meshRenderer = meshParent->GetComponent<MeshRenderComponent>();

					if (meshRenderer != nullptr)
					{
						_meshID = meshRenderer->_meshID;
						_instanceID = meshRenderer->_instanceID;
						vertexNum = meshRenderer->vertexNum;
						indexNum = meshRenderer->indexNum;
						meshRenderer->_meshID = -1;
					}

					meshParent->Destroy();

					std::string popUpmessage = "Loaded Mesh: " + drop;
					Application::Instance()->layers->editor->PopUpMessage(popUpmessage);

				}
				ImGui::EndDragDropTarget();
			}

			return;
		}

		Mesh& mesh = GetMesh();
		bool auxiliaryBool = _isEnabled;
		if (ImGui::Checkbox("Active", &auxiliaryBool))
			Enable(auxiliaryBool, false);

		ImGui::TextWrapped("Mesh vertices: "); ImGui::SameLine(); 
		ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(vertexNum).c_str());

		ImGui::TextWrapped("Mesh indices: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(indexNum).c_str());

		if (ImGui::BeginCombo("Show normals", comboValues[selectedNormalDisplay].c_str()))
		{
			for (int n = 0; n < 3; n++)
			{
				const bool is_selected = (selectedNormalDisplay == n);
				if (ImGui::Selectable(comboValues[n].c_str(), is_selected))
				{
					selectedNormalDisplay = n;
					mesh.showNormals = selectedNormalDisplay - 1;
				}
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
}

void MeshRenderComponent::Enable(bool enabled, bool fromGo)
{
	if (!fromGo) _isEnabled = enabled;
	GetMesh().draw = _isEnabled;

	if (_isEnabled && _gameObject->IsActive())
		GetMesh().draw = true;
	else if (_isEnabled && !_gameObject->IsActive())
		GetMesh().draw = false;
}
