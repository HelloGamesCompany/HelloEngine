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

	bool Start() override;

	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;

	bool CleanUp() override;
	
	static void S_DrawLayers();

	static void S_DrawEditor();

	static uint S_AddGameObject(GameObject* go, uint ID = 0);

	static void S_RequestLoadScene(const std::string& scenePath);

public:
	static LayerGame* game;

	static GameObject* rootGameObject;

	static LayerEditor* editor;

	static std::map<uint, GameObject*> gameObjects;

private:
	static Layer* _layers[(uint)LayersID::MAX];

	static bool _requestScene;

	static std::string _requestScenePath;

	static std::string _sceneBeginPath;

	static std::vector<GameObject*> _deletedGameObjects;

	friend class GameObject;
};