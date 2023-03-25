#pragma once
#include "ComponentUI.h"

class ComponentUIImage : public ComponentUI
{
	public:

	ComponentUIImage(GameObject* gameObject);
	~ComponentUIImage();

	void InputUpdate() override;

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;
	float _fillImage;
	float _maxScale;

private: 
	float _auxFillImage;
	ImWindowGame* _gameWindow = nullptr;

#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

