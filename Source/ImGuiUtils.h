#pragma once

namespace ImGui
{
	void HelpMarker(const char* helpText);
	void TextURL(const char* name_, const char* URL_, uint8_t SameLineBefore_, uint8_t SameLineAfter_);
	void AddUnderLine(ImColor col_);
}