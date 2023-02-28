#pragma once
#include "ComponentUI.h"

enum class SliderState {
	NORMAL,
	ONPRESS,
	ONHOLD,
	ONUNHOLD
};

class ComponentUISlider : public ComponentUI
{
public:
	ComponentUISlider(GameObject* gameObject);
	~ComponentUISlider();

	void InputUpdate() override;

	SliderState State;

	float widthSlider;
	float heightSlider;

private:
	bool isFocused = false;
};

