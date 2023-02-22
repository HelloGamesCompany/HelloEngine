#pragma once
#include "ComponentUI.h"

class ComponentUIButton : public ComponentUI
{
	ComponentUIButton(GameObject* gameObject);
	~ComponentUIButton();

	void InputUpdate() override;

private:
	bool isFocused = false;
};

