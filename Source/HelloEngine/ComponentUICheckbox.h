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

	CheckboxState State;
private:
	bool checkActive = false;

#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

