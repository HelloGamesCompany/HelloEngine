#pragma once
#include "ImWindow.h"
class ImWindowInspector : public ImWindow
{
public:
	ImWindowInspector();

	~ImWindowInspector();

	void Update() override;
};