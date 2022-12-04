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

	void UpdateFileNodes();

private:
	void DrawTreeNodePanelLeft(Directory*& newDir, Directory* node, const bool drawFiles = true) const;

	void DrawTreeNodePanelRight(Directory*& newDir);

	void OnDrop(const std::string filePath);

	void CheckWindowFocus();

private:
	Application* _app = nullptr;

	ImGuiTextFilter _filter;

	Directory* _rootNode = nullptr;

	File* _deleteFile = nullptr;

	FileTree* _fileTree = nullptr;

	std::string _dragPath = "";

	uint _dragUID = 0;

	SDL_Window* _window = nullptr;

	bool _isWindowFocus = false;

	int itemWidth = 80;
	int itemHeight = 80;

	uint fileImageID = 0;
	uint folderImageID = 0;
	uint modelImageID = 0;
	uint meshImageID = 0;
};