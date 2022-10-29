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
};