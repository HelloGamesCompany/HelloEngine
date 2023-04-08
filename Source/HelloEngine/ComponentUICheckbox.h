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

	void UpdateGamePadInput(std::vector<ComponentUI*>& _listButtons, int buttonSelected) override;

	CheckboxState ChangeState(CheckboxState State);

	bool checkActive = false;
	CheckboxState State;
private:
	float colors[4] = { 0,0,0,0 };

	int textureIDIdleCB = -1;
	int textureIDHoverCB = -1;
	int textureIDPressCB = -1;
	int textureIDActiveCB = -1;

	ResourceTexture* idleCB = nullptr;
	ResourceTexture* hoverCB = nullptr;
	ResourceTexture* pressCB = nullptr;
	ResourceTexture* activeCB = nullptr;

#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

