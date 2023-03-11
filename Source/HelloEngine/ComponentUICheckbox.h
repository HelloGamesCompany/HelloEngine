#pragma once
#include "ComponentUI.h"
#include "ModuleResourceManager.h"

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

	int textureIDIdleCB = -1;
	int textureIDHoverCB = -1;
	int textureIDPressCB = -1;

	ResourceTexture* idleCB = nullptr;
	ResourceTexture* hoverCB = nullptr;
	ResourceTexture* pressCB = nullptr;

#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

