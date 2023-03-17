#pragma once
#include "ComponentUI.h"
#include "ComponentUIButton.h"

class ComponentUIInput : public ComponentUI
{
	public:

	ComponentUIInput(GameObject* gameObject);
	~ComponentUIInput();

	void InputUpdate() override;

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	std::vector<ComponentUIButton *> _listButtons;

private: 
	ImWindowGame* _gameWindow = nullptr;

#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

