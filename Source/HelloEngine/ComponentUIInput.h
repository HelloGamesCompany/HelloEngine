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

	bool _isComponentEnable;

	ImWindowGame* _gameWindow = nullptr;
	bool isPress = true;
	int ButtonSelected = 0;

#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

