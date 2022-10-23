#pragma once
#include "ImWindow.h"
#include "imgui.h"

#include "FileTree.hpp"

class ImWindowProject : public ImWindow
{
public:
	ImWindowProject();

	~ImWindowProject();

	void Update() override;

private:
	void DrawTreeNode(const FileTree node)const;

private:

	Application* app = nullptr;

	ImGuiTextFilter filter;

	FileTree fileTree;

	FileTree currentNode;
};