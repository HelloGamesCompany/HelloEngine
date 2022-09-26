#include "Headers.h"

void ImGui::HelpMarker(const char* helpText)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(helpText);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
