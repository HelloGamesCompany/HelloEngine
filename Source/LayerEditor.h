#ifndef __MODULE_ENGINE_H__
#define __MODULE_ENGINE_H__

#include "ImWindow.h"
#include "Layer.h"
#include "LayerGame.h"
#include "FileTree.hpp"

class Application;
class ImWindowConfiguration;

enum class ImWindowID
{
	CONFIGURATION,
	PERFORMANCE,
	CONSOLE,
	PROJECT,
	HIERARCHY,
	INSPECTOR,
	QUICKSAVE,
	ABOUT,
	GAME,
	SCENE,
	RESOURCES,
	MAX,
};

struct PopUpMessage
{
	PopUpMessage(std::string message) :message(message) {}

	std::string message = "";
	
	float currentMessageTime = 0.0f;
	bool hovered = false;
};

class LayerEditor :public Layer
{
public:
	LayerEditor();
	~LayerEditor();

	void Start() override;

	void PreUpdate() override;
	void Update() override;
	void PostUpdate() override;

	void CleanUp() override;

	void SetSelectGameObject(GameObject* g);
	GameObject* GetSelectedGameObject() 
	{
		return selectedGameObject; 
	}

	void AddPopUpMessage(std::string message);
private:
	void DrawMenuBar();

	// Draw scene open and save popups
	void DrawPopUpLoadScene();
	void DrawPopUpSaveScene();
	void DrawAssetsTree(Directory*& newDir, Directory* node, const bool drawFiles);

	void DrawPopUpMessages();

public:
	ImWindowConfiguration* configuration = nullptr;
	GameObject* selectedGameObject = nullptr;

private:
	ImWindow* _imWindows[(uint)ImWindowID::MAX] = { nullptr };
	LayerGame* _game = nullptr;

	Application* _app = nullptr;

	int _gameWidth = 0, _gameHeight = 0;

	//Popup message variables
	std::vector<PopUpMessage> _popUpMessages;
	float _messageTime = 3.0f;

	// Scene popups
	bool _openLoadScene = false;
	bool _openSaveScene = false;
	FileTree* _fileTree = nullptr;
	std::string _currentSelectedPath = "";
	std::string _savingSceneName = "";
	bool _requestUpdateFileTree = false;
	friend class ImWindowHierarchy;
};

#endif // !__MODULE_ENGINE_H__