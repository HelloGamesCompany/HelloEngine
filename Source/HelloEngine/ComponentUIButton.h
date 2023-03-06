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

	ButtonState State;
private:
	double gameTimeCopy = 0;
	float colors[4] = { 0,0,0,0 };
	GameObject* uiObject;
	int textureID = -1;

	MeshRenderComponent* meshRenderer = nullptr;
	ResourceTexture* currentResource = nullptr;



#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

