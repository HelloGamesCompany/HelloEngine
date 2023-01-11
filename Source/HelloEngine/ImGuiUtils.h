#pragma once

namespace ImGui
{
	void HelpMarker(const char* helpText);
	void TextURL(const char* name_, const char* URL_, uint8_t SameLineBefore_, uint8_t SameLineAfter_);
	void AddUnderLine(ImColor col_);
	void DrawSplitter(int split_vertically, float thickness, float* size0, float* size1, float min_size0, float min_size1);

	IMGUI_API bool  InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
	IMGUI_API bool  InputTextMultiline(const char* label, std::string* str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
	IMGUI_API bool  InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
}