#pragma once
#include "ComponentUI.h"
#include "ComponentUIButton.h"
#include "ComponentUISlider.h"
#include "ComponentUICheckbox.h"

class ComponentUIInput : public ComponentUI
{
	public:

	ComponentUIInput(GameObject* gameObject);
	~ComponentUIInput();

	void InputUpdate() override;

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	std::vector<ComponentUI *> _listButtons;
	bool _isComponentEnable;

private: 


	ImWindowGame* _gameWindow = nullptr;
	bool isPress = true;
	bool AisPress = true;
	int ButtonSelected = 0;

#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

