#pragma once
#include "Module.h"
#include "Layer.h"
#include "GameObject.h"

class LayerEditor;
class LayerGame;

enum LayersID
{
	GAME,
	UI,
	DEBUG,
	EDITOR,// Editor should always be the last layer
	MAX
};

class ModuleLayers : public Module
{
public:
	ModuleLayers();
	~ModuleLayers();

	bool Init() override;

	bool Start() override;

	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;

	void DrawLayers();

	void DrawEditor();

	bool CleanUp() override;

	uint AddGameObject(GameObject* go, uint ID = 0);

	void RequestLoadScene(const std::string& scenePath);

public:
	Layer* layers[(uint)LayersID::MAX] = { nullptr };

	LayerGame* game = nullptr;

	GameObject* rootGameObject = nullptr;

	LayerEditor* editor = nullptr;

	std::map<uint, GameObject*> gameObjects;

private:
	bool _requestScene = false;

	std::string _requestScenePath = "null";

	std::string _sceneBeginPath = "null";

	std::vector<GameObject*> _deletedGameObjects;

	friend class GameObject;
};

