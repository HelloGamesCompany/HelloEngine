#pragma once
#include "Module.h"
#include "Layer.h"

enum LayersID
{
	GAME,
	UI,
	DEBUG,
	EDITOR,// Editor should always be the last layer
	MAX
};

class ModuleLayers : public Module
{
public:
	ModuleLayers();
	~ModuleLayers();

	bool Start() override;

	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;

	void DrawLayers();

	void DrawEditor();

	bool CleanUp() override;

public:
	Layer* layers[(uint)LayersID::MAX] = { nullptr };
};

