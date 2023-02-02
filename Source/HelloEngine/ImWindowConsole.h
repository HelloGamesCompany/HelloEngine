#pragma once

#include "ImWindow.h"
class ImWindowConsole : public ImWindow
{
public:
	ImWindowConsole();

	~ImWindowConsole();

	void Update() override;

private:

	bool _isCollapse = false;
	ImVec4 textColors[3] = { {1,1,1,1}, {1,1,0,1}, {1,0,0,1} };
};