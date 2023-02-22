#pragma once
#include "Layer.h"

class ComponentUI;

class LayerUI : public Layer
{
public:

	LayerUI();
	~LayerUI();

	void Start() override;

	void PreUpdate() override;
	void Update() override;
	void PostUpdate() override;
	void CleanUp() override;


	static void AddUI(uint UID, ComponentUI* component);
	static void RemoveUI(uint gameObjectID);

private:

	static std::map<uint, ComponentUI*> _componentUIs;
};

