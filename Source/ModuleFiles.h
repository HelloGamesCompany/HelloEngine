#ifndef __MODULE_PHYSFS_H__
#define __MODULE_PHYSFS_H__

#include "Module.h"

#define PHYSFS_PATH "hello"

class ModuleFiles : public Module
{
public:
	ModuleFiles();

	~ModuleFiles();

	void ReadTest();
};

#endif // !__MODULE_PHYSFS_H__