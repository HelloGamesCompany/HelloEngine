#pragma once
#include "ComponentUI.h"

enum class ButtonState {
	NORMAL,
	HOVERED,
	ONPRESS,
	ONHOLD
};

class ComponentUIButton : public ComponentUI
{
	public:

	ComponentUIButton(GameObject* gameObject);
	~ComponentUIButton();

	void InputUpdate() override;
	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	ButtonState State;
private:
	double gameTimeCopy = 0;
	float colors[4] = { 0,0,0,0 };
#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

