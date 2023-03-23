#pragma once
#include "Component.h"
#include "Globals.h"

class MeshRenderComponent;
class TextureComponent;
class ImWindowGame;


class ComponentUI : public Component
{
public:

	ComponentUI(GameObject* gameObject);
	~ComponentUI();

	bool IsMouseOver();

	virtual void Serialization(json& j);
	virtual void DeSerialization(json& j);

	// This is a method to check all input necessary on the given UI. It is called at the PreUpdate method of LayerUI.
	virtual void InputUpdate() { IsMouseOver(); };

private:
#ifdef STANDALONE
	ImWindowGame* _gameWindow = nullptr;
#endif
protected:

	MeshRenderComponent* _meshRenderer = nullptr;
	TextureComponent* _material = nullptr;

	uint _UUID = 0;
};

