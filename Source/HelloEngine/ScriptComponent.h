#pragma once
#include "Component.h"
#include "ModuleResourceManager.h"

class ScriptComponent : public Component
{
public:
	ScriptComponent(GameObject* go);
	~ScriptComponent();


private:
	uint scriptUID;
	ResourceScript* scriptResource = nullptr;

	friend class LayerGame;
};

