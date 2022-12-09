#include "Headers.h"
#include "MaterialComponent.h"
#include "InstanceRenderer.h"
#include "GameObject.h"
#include "TextureImporter.h"
#include "ModuleResourceManager.h"
#include "TextureManager.h"

MaterialComponent::MaterialComponent(GameObject* go) : Component(go)
{
	_type = Component::Type::MATERIAL;
	MeshRenderComponent* meshRenderer = go->GetComponent<MeshRenderComponent>();
	if (!meshRenderer) return;
	SetMeshRenderer(meshRenderer);
}

MaterialComponent::~MaterialComponent()
{
	if (currentResource != nullptr)
		currentResource->Dereference();
}

Mesh& MaterialComponent::GetMesh()
{
	return meshRenderer->GetMesh();
}

void MaterialComponent::ChangeTexture(ResourceTexture* resource)
{
	if (resource == nullptr)
	{
		textureID = -1.0f;
		currentResource = nullptr;
		return;
	}
	this->textureID = resource->OpenGLID;

	if (currentResource != nullptr)
		currentResource->Dereference();

	currentResource = resource;

	/*if (resource->isTransparent)
		meshRenderer->SetMeshAsTransparent();*/

	GetMesh().textureID = textureID;
}

void MaterialComponent::ChangeTexture(int ID)
{
	this->textureID = ID;

	GetMesh().textureID = textureID;
}

void MaterialComponent::MarkAsDead()
{
	if (currentResource != nullptr)
	{
		currentResource->Dereference();
		resourceUID = currentResource->UID;
		currentResource = nullptr;
	}
}

void MaterialComponent::MarkAsAlive()
{
	ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(resourceUID));
}

void MaterialComponent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["ResourceUID"] = currentResource ? currentResource->UID : 0;
	j["Components"].push_back(_j);
}

void MaterialComponent::DeSerialization(json& j)
{
	uint savedUID = j["ResourceUID"];
	if (savedUID == CHECKERS_RESOURCE_UID)
	{
		ChangeTexture(TextureImporter::CheckerImage());
		return;
	}

	ResourceTexture* resource = savedUID == 0 ? nullptr : (ResourceTexture*)ModuleResourceManager::S_LoadResource(j["ResourceUID"]);
	ChangeTexture(resource);
}


void MaterialComponent::OnEditor()
{
	if (!ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) return;

	bool auxiliaryBool = _isEnabled;
	if (ImGui::Checkbox("Active##Material", &auxiliaryBool))
		auxiliaryBool ? Enable() : Disable();

	if (!meshRenderer)
	{
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "No MeshRenderComponent detected!");

		if (ImGui::Button("Search MeshRenderComponent"))
		{
			MeshRenderComponent* meshRenderer = _gameObject->GetComponent<MeshRenderComponent>();
			if (!meshRenderer) return;
			SetMeshRenderer(meshRenderer);
		}
	}
	else
	{
		Mesh& mesh = GetMesh();

		if (ImGui::Button("Set Checkers Texture"))
		{
			ChangeTexture(TextureImporter::CheckerImage());
		}

		std::string imageName;
		int width = 0;
		int height = 0;
		if (textureID != -1.0f && currentResource != nullptr)
		{
			ImGui::Image((ImTextureID)(uint)textureID, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
			
			imageName = currentResource->debugName;
			width = currentResource->width;
			height = currentResource->height;
		}
		else 
		{
			ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
			imageName = "None";
		}
		
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
			{
				//Drop asset from Asset window to scene window
				const uint* drop = (uint*)payload->Data;

				ResourceTexture* resource = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);

				ChangeTexture(resource);
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::TextWrapped("Path: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), imageName.c_str());

		ImGui::TextWrapped("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(width).c_str());

		ImGui::TextWrapped("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(height).c_str());
	}
}

void MaterialComponent::OnEnable()
{
	if (!meshRenderer) return;
	GetMesh().textureID = textureID;
}

void MaterialComponent::OnDisable()
{
	if (!meshRenderer) return;
	GetMesh().textureID = -1;
}

void MaterialComponent::SetMeshRenderer(MeshRenderComponent* mesh)
{
	this->meshRenderer = mesh;
	this->textureID = GetMesh().textureID;
}
