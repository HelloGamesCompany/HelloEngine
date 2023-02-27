#pragma once
#include "ComponentUI.h"

enum class ButtonState {
	NORMAL,
	ONPRESS
};

class ComponentUIButton : public ComponentUI
{
	public:

	ComponentUIButton(GameObject* gameObject);
	~ComponentUIButton();

	void InputUpdate() override;
	ButtonState State;
private:
	bool isFocused = false;
};

