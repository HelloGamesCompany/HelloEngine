#include "Headers.h"
#include "ImWindowResources.h"
#include "ModuleResourceManager.h"

ImWindowResources::ImWindowResources()
{
	windowName = "Resource Testing";
}

ImWindowResources::~ImWindowResources()
{
}

void ImWindowResources::Update()
{
	if (ImGui::Begin(windowName.c_str()))
	{
		ImGui::TextWrapped("Models are not Resources, only Meshes. Mesh Resources get created when loaded, instead of when imported, because we can only import Models, not Meshes.");
		if (ImGui::CollapsingHeader("Meshes"))
		{
			for (auto& resource : ModuleResourceManager::resources)
			{
				if (resource.second->type == ResourceType::MESH)
				{
					ImGui::Text(resource.second->debugName.c_str()); ImGui::SameLine();
					ImGui::TextColored(ImVec4(1, 0, 1, 1), std::to_string(resource.second->referenceCount).c_str());
				}
			}
		}
		if (ImGui::CollapsingHeader("Textures"))
		{
			for (auto& resource : ModuleResourceManager::resources)
			{
				if (resource.second->type == ResourceType::TEXTURE) // Ignore MODEL resources, because their reference count is not necessary to track, only meshes's.
				{
					ImGui::Text(resource.second->debugName.c_str()); ImGui::SameLine();
					ImGui::TextColored(ImVec4(1, 0, 1, 1), std::to_string(resource.second->referenceCount).c_str());
				}
			}
		}
	}
	ImGui::End();
}
