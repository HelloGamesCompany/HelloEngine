#pragma once
#include "ComponentUI.h"
#include "ModuleResourceManager.h"

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

	void UpdateGamePadInput() override;

	ButtonState ChangeState(ButtonState State);

	ButtonState State;
	bool IsHold = false;
private:
	double gameTimeCopy = 0;
	float colors[4] = { 0,0,0,0 };
	GameObject* uiObject;
	int textureIDIdle = -1;
	int textureIDHover = -1;
	int textureIDPress = -1;

	MeshRenderComponent* meshRenderer = nullptr;
	ResourceTexture* currentResource = nullptr;

	ResourceTexture* idleButton = nullptr;
	ResourceTexture* hoverButton = nullptr;
	ResourceTexture* pressButton = nullptr;



#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

