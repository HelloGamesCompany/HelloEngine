#pragma once
#include "Module.h"
#include "Layer.h"

enum LayersID
{
	GAME,
	UI,
	DEBUG,
	EDITOR,
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

	bool CleanUp() override;

private:
	Layer* layers[(uint)LayersID::MAX] = { nullptr };
};

