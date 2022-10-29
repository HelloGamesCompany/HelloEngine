#pragma once

#include "ImWindow.h"
class ImWindowConsole : public ImWindow
{
public:
	ImWindowConsole();

	~ImWindowConsole();

	void Update() override;

	void FocusConsole();

private:
	bool focus = false;
	bool _isCollapse = false;
};