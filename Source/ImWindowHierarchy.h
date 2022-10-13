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

	void DrawGameObjectChildren(GameObject* gameObject, int layer);

private:
	std::map<uint, GameObject*>* gameObjectsReference = nullptr;
	LayerEditor* layerEditor = nullptr;

	GameObject* draggingGameObject = nullptr;

	ImGuiTreeNodeFlags base_flags;

};

