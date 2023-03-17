#pragma once
#include "ComponentUI.h"

class ComponentUIInput : public ComponentUI
{
	public:

	ComponentUIInput(GameObject* gameObject);
	~ComponentUIInput();

	void InputUpdate() override;

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

private: 
	ImWindowGame* _gameWindow = nullptr;

#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

