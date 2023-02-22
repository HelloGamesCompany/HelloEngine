#pragma once
#include "Component.h"

class MeshRenderComponent;
class MaterialComponent;
class ImWindowGame;

class ComponentUI : public Component
{
public:

	ComponentUI(GameObject* gameObject);
	~ComponentUI();

	bool IsMouseOver();

	void Serialization(json& j);
	void DeSerialization(json& j);

	// This is a method to check all input necessary on the given UI. It is called at the PreUpdate method of LayerUI.
	virtual void InputUpdate() { IsMouseOver(); };

private:
#ifdef STANDALONE
	ImWindowGame* _gameWindow = nullptr;
#endif
	MeshRenderComponent* _meshRenderer = nullptr;
	MaterialComponent* _material = nullptr;

	uint _UUID = 0;
};

