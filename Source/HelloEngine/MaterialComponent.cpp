#include "Headers.h"
#include "MaterialComponent.h"
#include "InstanceRenderer.h"
#include "GameObject.h"
#include "TextureImporter.h"
#include "ModuleResourceManager.h"
#include "TextureManager.h"
#include "SkinnedMeshRenderComponent.h"

MaterialComponent::MaterialComponent(GameObject* go) : Component(go)
{
	_type = Component::Type::MATERIAL;
	MeshRenderComponent* meshRenderer = go->GetComponent<MeshRenderComponent>();
	SkinnedMeshRenderComponent* skinnedMeshRenderer;
	if (!meshRenderer)
	{
		SkinnedMeshRenderComponent* skinnedMeshRenderer = go->GetComponent<SkinnedMeshRenderComponent>();
		if(!skinnedMeshRenderer) return;

		SetSkinnedMeshRenderer(skinnedMeshRenderer);
	} else SetMeshRenderer(meshRenderer);
}

MaterialComponent::~MaterialComponent()
{
	if (currentResource != nullptr)
		currentResource->Dereference();
	if (meshRenderer != nullptr)
		ChangeTexture(nullptr);
}

Mesh& MaterialComponent::GetMesh()
{
	if (skinnedMeshRenderer != nullptr) return skinnedMeshRenderer->GetMesh();
	
	return meshRenderer->GetMesh();
}

void MaterialComponent::ChangeTexture(ResourceTexture* resource)
{
	if (resource == nullptr)
	{
		textureID = -1.0f;
		currentResource = nullptr;

		if (_gameObject->HasComponent<MeshRenderComponent>())
		{
			if (meshRenderer != nullptr)
				GetMesh().textureID = textureID;
		}
		return;
	}
	this->textureID = resource->OpenGLID;

	if (currentResource != nullptr)
		currentResource->Dereference();

	currentResource = resource;

	if (resource->isTransparent)
		meshRenderer->ChangeMeshRenderType(MeshRenderType::TRANSPARENCY);

	GetMesh().textureID = textureID;
}

void MaterialComponent::ChangeTexture(int ID)
{
	this->textureID = ID;

	GetMesh().textureID = textureID;
}

#ifdef STANDALONE
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
#endif // STANDALONE



void MaterialComponent::SetAsUI()
{
	isUI = true;
}

void MaterialComponent::Serialization(json& j)
{
	// We don't want to serialize UI Materials because their information is saved inside the ComponentUI. We assume every UI Material is attached to a ComponentUI.
	if (isUI)
		return;

	json _j;

	_j["Type"] = _type;
	_j["ResourceUID"] = currentResource ? currentResource->UID : 0;
	_j["Enabled"] = _isEnabled;
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

	if (_gameObject->HasComponent<MeshRenderComponent>())
	{
		SetMeshRenderer(_gameObject->GetComponent<MeshRenderComponent>());
	}
	else
	{
		SetSkinnedMeshRenderer(_gameObject->GetComponent<SkinnedMeshRenderComponent>());
	}

	

	if (meshRenderer != nullptr || skinnedMeshRenderer)
	{
		ResourceTexture* resource = savedUID == 0 ? nullptr : (ResourceTexture*)ModuleResourceManager::S_LoadResource(j["ResourceUID"]);
		ChangeTexture(resource);
	}

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();
}

void MaterialComponent::UpdateMaterial()
{
	GetMesh().textureID = textureID;
}

uint MaterialComponent::GetResourceUID()
{
	if (currentResource != nullptr)
		return currentResource->UID;
	return 0;
}

void MaterialComponent::DestroyedResource()
{
	ChangeTexture(nullptr);
}
#ifdef STANDALONE
void MaterialComponent::OnEditor()
{
	bool created = true;
	if (!ImGui::CollapsingHeader("Material", &created, ImGuiTreeNodeFlags_DefaultOpen)) return;
	if (!created)
	{
		_gameObject->DestroyComponent(this);
		return;
	}
	bool auxiliaryBool = _isEnabled;
	if (ImGui::Checkbox("Active##Material", &auxiliaryBool))
		auxiliaryBool ? Enable() : Disable();

	if (!meshRenderer && !skinnedMeshRenderer)
	{
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "No MeshRenderComponent detected!");

		if (ImGui::Button("Search MeshRenderComponent"))
		{
			MeshRenderComponent* meshRenderer = _gameObject->GetComponent<MeshRenderComponent>();
			if (!meshRenderer)
			{
				SkinnedMeshRenderComponent* skinnedMeshRenderer = _gameObject->GetComponent<SkinnedMeshRenderComponent>();
				if (!skinnedMeshRenderer) return;

				SetSkinnedMeshRenderer(skinnedMeshRenderer);
			} else 	SetMeshRenderer(meshRenderer);
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
#endif // STANDALONE

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
	if (meshRenderer == nullptr)
	{
		textureID = -1;
		return;
	}
}

void MaterialComponent::SetSkinnedMeshRenderer(SkinnedMeshRenderComponent* mesh)
{
	this->skinnedMeshRenderer = mesh;
	if (meshRenderer == nullptr)
	{
		textureID = -1;
		return;
	}
}
