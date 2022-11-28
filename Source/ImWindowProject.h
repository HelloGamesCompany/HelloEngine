#pragma once
#include "ImWindow.h"
#include "imgui.h"

class FileTree;
class File;
struct Directory;

class ImWindowProject : public ImWindow
{
public:
	ImWindowProject();

	~ImWindowProject();

	void Update() override;

private:
	void DrawTreeNodePanelLeft(Directory*& newDir, Directory* node, const bool drawFiles = true) const;

	void DrawTreeNodePanelRight(Directory*& newDir);

	void OnDrop(const std::string filePath);

	void UpdateFileNodes();

	void CheckWindowFocus();

private:
	Application* _app = nullptr;

	ImGuiTextFilter _filter;

	Directory* _rootNode = nullptr;

	File* _deleteFile = nullptr;

	FileTree* _fileTree = nullptr;

	std::string _dragPath = "";

	SDL_Window* _window = nullptr;

	bool _isWindowFocus = false;
};