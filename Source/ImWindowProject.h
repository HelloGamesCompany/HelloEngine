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

	void PanelCreateFolder();

private:
	Application* _app = nullptr;

	ImGuiTextFilter _filter;

	Directory* _rootNode = nullptr;

	File* _deleteFile = nullptr;

	Directory* _deleteDir = nullptr;

	FileTree* _fileTree = nullptr;

	std::string _dragPath = "";

	std::string _createFolderName = "folder";

	uint _dragUID = 0;

	SDL_Window* _window = nullptr;

	bool _isWindowFocus = false;

	bool _opemChangeNamePanel = false;

	int _itemWidth = 80;

	int _itemHeight = 80;

	uint _fileImageID = 0;
	uint _folderImageID = 0;
	uint _modelImageID = 0;
	uint _meshImageID = 0;
	uint _sceneImageID = 0;
	uint _textureImageID = 0;
};