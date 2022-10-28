#include "Headers.h"
#include "MaterialComponent.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "TextureImporter.h"
#include "ModuleResourceManager.h"

MaterialComponent::MaterialComponent(GameObject* go) : Component(go)
{
}

MaterialComponent::~MaterialComponent()
{
}

Mesh& MaterialComponent::GetMesh()
{
	return meshRenderer->GetMesh();
}

void MaterialComponent::OnEditor()
{
	if (!ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) return;

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
			mesh.textureID = TextureImporter::CheckerImage();
		}

		if (mesh.textureID != -1.0f)
			ImGui::Image((ImTextureID)(uint)mesh.textureID, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
		else
			ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
			{
				//Drop asset from Asset window to scene window
				const std::string drop = *(std::string*)payload->Data;

				Resource* resource = Application::Instance()->resource->LoadFile(drop);

				if (resource->type != ResourceType::TEXTURE) return;

				ResourceTexture* textureResource = (ResourceTexture*)resource;

				mesh.textureID = textureResource->textureInfo.OpenGLID;

				RELEASE(resource);
			}
			ImGui::EndDragDropTarget();
		}
	}
}

void MaterialComponent::SetMeshRenderer(MeshRenderComponent* mesh)
{
	this->meshRenderer = mesh;
}

void MaterialComponent::ChangeTexture(uint textureID)
{
	GetMesh();
}
