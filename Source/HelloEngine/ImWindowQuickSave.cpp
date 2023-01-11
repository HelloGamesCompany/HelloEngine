#include "Headers.h"
#include "ImWindowQuickSave.h"

ImWindowQuickSave::ImWindowQuickSave()
{
	windowName = "Quick Save";

	isEnabled = true;
}

ImWindowQuickSave::~ImWindowQuickSave()
{
}

void ImWindowQuickSave::Update()
{
	if (ImGui::Begin(windowName.c_str(), &isEnabled))
	{
		std::string label;

		if (ImGui::CollapsingHeader("Bool", ImGuiTreeNodeFlags_DefaultOpen))
		{
			pugi::xml_node nBool = QuickSave::GetRootNodeBool().node;

			for (nBool = nBool.first_child(); nBool != nullptr; nBool = nBool.next_sibling())
			{
				label = nBool.name();

				label += ": ";

				label += nBool.attribute("value").as_bool() ? "true" : "false";

				ImGui::Text(label.c_str());
			}
		}
		if (ImGui::CollapsingHeader("String", ImGuiTreeNodeFlags_DefaultOpen))
		{
			pugi::xml_node nString = QuickSave::GetRootNodeString().node;

			for (nString = nString.first_child(); nString != nullptr; nString = nString.next_sibling())
			{
				label = nString.name();

				label += ": ";

				label += nString.attribute("value").as_string();

				ImGui::Text(label.c_str());
			}
		}
		if (ImGui::CollapsingHeader("Int", ImGuiTreeNodeFlags_DefaultOpen))
		{
			pugi::xml_node nInt = QuickSave::GetRootNodeInt().node;

			for (nInt = nInt.first_child(); nInt != nullptr; nInt = nInt.next_sibling())
			{
				label = nInt.name();

				label += ": ";

				label += nInt.attribute("value").as_string();

				ImGui::Text(label.c_str());
			}
		}
		if (ImGui::CollapsingHeader("Float", ImGuiTreeNodeFlags_DefaultOpen))
		{
			pugi::xml_node nFloat = QuickSave::GetRootNodeFloat().node;		

			for (nFloat = nFloat.first_child(); nFloat != nullptr; nFloat = nFloat.next_sibling())
			{
				label = nFloat.name();

				label += ": ";

				label += nFloat.attribute("value").as_string();

				ImGui::Text(label.c_str());
			}
		}
	}
	ImGui::End();
}