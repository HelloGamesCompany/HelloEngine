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
	std::map<uint, GameObject*>* gameObjectsReference = nullptr;

	LayerEditor* layerEditor = nullptr;

	GameObject* draggingGameObject = nullptr;

	ImGuiTreeNodeFlags base_flags = 0;

	bool popUpOpen = false;
	bool hasSelectedAGameObject = false;
};

