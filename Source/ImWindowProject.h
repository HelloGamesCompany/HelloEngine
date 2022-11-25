#pragma once
#include "ImWindow.h"
#include "imgui.h"

class FileTree;
struct Directory;

class ImWindowProject : public ImWindow
{
public:
	ImWindowProject();

	~ImWindowProject();

	void Update() override;

private:
	void DrawTreeNode(const Directory* node, bool drawFiles = true) const;

	void OnDrop(const std::string filePath);

	void UpdateFileNodes();

	void CheckWindowFocus();

private:
	Application* _app = nullptr;

	ImGuiTextFilter filter;

	Directory* _rootNode = nullptr;

	FileTree* _fileTree = nullptr;

	std::string _dragPath = "";

	SDL_Window* _window = nullptr;

	ModuleResourceManager* _resource = nullptr;

	bool _isWindowFocus = false;
};