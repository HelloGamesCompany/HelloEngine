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
		std::map<std::string, uint> alphabeticallyOrderedResources;

		for (auto& resource : ModuleResourceManager::resources)
		{
			alphabeticallyOrderedResources[resource.second->debugName] = resource.first;
		}

		for (auto& resource : alphabeticallyOrderedResources)
		{
			if (ModuleResourceManager::resources[resource.second]->type != ResourceType::MODEL) // Ignore MODEL resources, because their reference count is not necessary to track, only meshes's.
			{
				ImGui::Text(ModuleResourceManager::resources[resource.second]->debugName.c_str()); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 0, 1, 1), std::to_string(ModuleResourceManager::resources[resource.second]->referenceCount).c_str());
			}
		}
	}
	ImGui::End();
}
