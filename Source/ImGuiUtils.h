#pragma once

namespace ImGui
{
	void HelpMarker(const char* helpText);
	void TextURL(const char* name_, const char* URL_, uint8_t SameLineBefore_, uint8_t SameLineAfter_);
	void AddUnderLine(ImColor col_);
	void DrawSplitter(int split_vertically, float thickness, float* size0, float* size1, float min_size0, float min_size1);
}