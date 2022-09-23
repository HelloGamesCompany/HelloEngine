#ifndef __MODULE_ENGINE_H__
#define __MODULE_ENGINE_H__

#include "Module.h"
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
};

#endif // !__MODULE_ENGINE_H__