#pragma once
#include "ComponentUI.h"

enum class CheckboxState {
	NORMAL,
	HOVERED,
	HOVEREDACTIVE,
	ONPRESS,
	ACTIVE
};

class ComponentUICheckbox : public ComponentUI
{
	public:

	ComponentUICheckbox(GameObject* gameObject);
	~ComponentUICheckbox();

	void InputUpdate() override;
	void Serialization(json& j) override;
	void DeSerialization(json& j) override;
	CheckboxState ChangeState(CheckboxState State);

	CheckboxState State;
private:
	bool checkActive = false;
	float colors[4] = { 0,0,0,0 };
#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

