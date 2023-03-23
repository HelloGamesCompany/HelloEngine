#include "Headers.h"
#include "TextureComponent.h"
#include "InstanceRenderer.h"
#include "GameObject.h"
#include "TextureImporter.h"
#include "ModuleResourceManager.h"
#include "TextureManager.h"
#include "SkinnedMeshRenderComponent.h"

TextureComponent::TextureComponent(GameObject* go) : Component(go)
{
	_type = Component::Type::TEXTURE;
	MeshRenderComponent* meshRenderer = go->GetComponent<MeshRenderComponent>();
	SkinnedMeshRenderComponent* skinnedMeshRenderer;
	if (!meshRenderer)
	{
		SkinnedMeshRenderComponent* skinnedMeshRenderer = go->GetComponent<SkinnedMeshRenderComponent>();
		if(!skinnedMeshRenderer) return;

		SetSkinnedMeshRenderer(skinnedMeshRenderer);
	} else SetMeshRenderer(meshRenderer);
}

TextureComponent::~TextureComponent()
{
    if (currentResource != nullptr)
        currentResource->Dereference();
    if (meshRenderer != nullptr)
        ChangeTexture(nullptr);
}

Mesh& TextureComponent::GetMesh()
{
	if (skinnedMeshRenderer != nullptr) return skinnedMeshRenderer->GetMesh();
	
	return meshRenderer->GetMesh();
}

void TextureComponent::ChangeTexture(ResourceTexture* resource)
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

	if (resource->isTransparent && !isUI)
		meshRenderer->ChangeMeshRenderType(MeshRenderType::TRANSPARENCY);

    GetMesh().textureID = textureID;
}

void TextureComponent::ChangeTexture(int ID)
{
    this->textureID = ID;

    GetMesh().textureID = textureID;
}

void TextureComponent::MarkAsDead()
{
    if (currentResource != nullptr)
    {
        currentResource->Dereference();
        resourceUID = currentResource->UID;
        currentResource = nullptr;
    }
}

void TextureComponent::MarkAsAlive()
{
    ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(resourceUID));
}



void TextureComponent::SetAsUI()
{
    isUI = true;
}

void TextureComponent::Serialization(json& j)
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

void TextureComponent::DeSerialization(json& j)
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

void TextureComponent::UpdateMaterial()
{
    GetMesh().textureID = textureID;
}

uint TextureComponent::GetResourceUID()
{
    if (currentResource != nullptr)
        return currentResource->UID;
    return 0;
}

void TextureComponent::DestroyedResource()
{
    ChangeTexture(nullptr);
}
void TextureComponent::OnEditor()
{
	bool created = true;
	if (!ImGui::CollapsingHeader("Texture", &created, ImGuiTreeNodeFlags_DefaultOpen)) return;
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

void TextureComponent::OnEnable()
{
    if (!meshRenderer) return;
    GetMesh().textureID = textureID;
}

void TextureComponent::OnDisable()
{
    if (!meshRenderer) return;
    GetMesh().textureID = -1;
}

void TextureComponent::SetMeshRenderer(MeshRenderComponent* mesh)
{
    this->meshRenderer = mesh;
    if (meshRenderer == nullptr)
    {
        textureID = -1;
        return;
    }
}

void TextureComponent::SetSkinnedMeshRenderer(SkinnedMeshRenderComponent* mesh)
{
	this->skinnedMeshRenderer = mesh;
	if (meshRenderer == nullptr)
	{
		textureID = -1;
		return;
	}
}
