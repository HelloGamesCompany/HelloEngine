#pragma once
#include "ImWindow.h"

class GameObject;
class LayerEditor;

class ImWindowHierarchy : public ImWindow
{
public:

	ImWindowHierarchy();

	~ImWindowHierarchy();

	void Update() override;

	void DrawGameObjectChildren(GameObject* gameObject, bool onlyChildren = false);

	void ProcessGameObject(GameObject* gameObject, int iteration);

private:

	void DrawOptions();

private:
	std::map<uint, GameObject*>* _gameObjectsReference = nullptr;

	LayerEditor* _layerEditor = nullptr;

	GameObject* _draggingGameObject = nullptr;

	ImGuiTreeNodeFlags _base_flags = 0;

	bool _popUpOpen = false;

	bool _hasSelectedAGameObject = false;

	Application* _app = nullptr;

	friend class GameObject;
};