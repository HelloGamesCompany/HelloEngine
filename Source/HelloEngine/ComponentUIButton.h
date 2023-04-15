#pragma once
#include "ComponentUI.h"
#include "ModuleResourceManager.h"

enum class ButtonState {
	NORMAL,
	HOVERED,
	ONPRESS,
	ONHOLD,
	BLOCKED
};

class ComponentUIButton : public ComponentUI
{
	public:

	ComponentUIButton(GameObject* gameObject);
	~ComponentUIButton();

	void InputUpdate() override;
	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	void UpdateGamePadInput(bool selected) override;

	ButtonState ChangeState(ButtonState State);

	ButtonState State;
	bool IsHold = false;

	bool isBlocked = false;

	bool isPress = true;
	bool isReleased = false;
private:
	double gameTimeCopy = 0;
	float colors[4] = { 0,0,0,0 };
	GameObject* uiObject;
	int textureIDIdle = -1;
	int textureIDHover = -1;
	int textureIDPress = -1;
	int textureIDBlocked = -1;

	MeshRenderComponent* meshRenderer = nullptr;
	ResourceTexture* currentResource = nullptr;

	ResourceTexture* idleButton = nullptr;
	ResourceTexture* hoverButton = nullptr;
	ResourceTexture* pressButton = nullptr;
	ResourceTexture* blockedButton = nullptr;

	bool AisPress = true;



#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

