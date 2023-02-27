#pragma once
#include "ComponentUI.h"

enum class CheckboxState {
	NORMAL,
	ONPRESS,
	ONHOLD,
	ONUNHOLD
};

class ComponentUICheckbox : public ComponentUI
{
	public:

	ComponentUICheckbox(GameObject* gameObject);
	~ComponentUICheckbox();

	void InputUpdate() override;
	CheckboxState State;
private:
	bool isFocused = false;
};

