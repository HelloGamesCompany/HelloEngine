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


private:

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	bool isFocused = false;
};

