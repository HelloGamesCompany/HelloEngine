#pragma once
#include "ImWindow.h"

class GameObject;

class ImWindowInspector : public ImWindow
{
public:
	ImWindowInspector();

	~ImWindowInspector();

	void Update() override;

	void SelectGameObject(GameObject* g);

private:

	GameObject* selectGameobject = nullptr;
};