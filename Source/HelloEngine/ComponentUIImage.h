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
};

