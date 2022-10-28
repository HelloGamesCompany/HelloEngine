#pragma once
#include "ImWindow.h"
#include "imgui.h"

class FileTree;

class ImWindowProject : public ImWindow
{
public:
	ImWindowProject();

	~ImWindowProject();

	void Update() override;

private:
	void DrawTreeNode(const FileTree* node, bool drawFiles = true)const;

	void OnDrop(std::string filePath);

private:

	Application* app = nullptr;

	ImGuiTextFilter filter;

	FileTree* fileTree = nullptr;

	FileTree* currentNode = nullptr;

	std::string newPath;
};