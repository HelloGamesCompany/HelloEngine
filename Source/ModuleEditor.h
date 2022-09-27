#ifndef __MODULE_ENGINE_H__
#define __MODULE_ENGINE_H__

#include "Module.h"
#include "ImWindow.h"

enum class ImWindowID
{
	CONFIGURATION,
	ABOUT,
	OPENGL,
	MAX,
};

class ModuleEditor :public Module
{
public:
	ModuleEditor(bool enabled);
	~ModuleEditor();

	bool Start() override;

	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;

	bool CleanUp() override;

private:

	ImWindow* imWindows[(uint)ImWindowID::MAX];
};

#endif // !__MODULE_ENGINE_H__