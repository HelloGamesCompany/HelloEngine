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

	// TODO: This should be inherited by any componentUI children.
	void Serialization(json& j);
	void DeSerialization(json& j);

private:
#ifdef STANDALONE
	ImWindowGame* _gameWindow = nullptr;
#endif
	MeshRenderComponent* _meshRenderer = nullptr;
	MaterialComponent* _material = nullptr;
};

