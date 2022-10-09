#pragma once
#include "ImWindow.h"
#include "imgui.h"

class ImWindowProject : public ImWindow
{
public:

	ImWindowProject();

	~ImWindowProject();

	void Update() override;

private:

	Application* app = nullptr;

	ImGuiTextFilter filter;
};