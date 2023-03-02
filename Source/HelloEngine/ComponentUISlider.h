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

	ImWindowGame* _gameWindow = nullptr;

	bool isFocused = false;

#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

