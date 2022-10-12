#pragma once
#include "ImWindow.h"

class GameObject;

class ImWindowInspector : public ImWindow
{
public:
	ImWindowInspector();

	~ImWindowInspector();

	void Update() override;

private:

	GameObject* selectGameobject = nullptr;
};