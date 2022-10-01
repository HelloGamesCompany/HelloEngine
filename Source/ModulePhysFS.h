#ifndef __MODULE_PHYSFS_H__
#define __MODULE_PHYSFS_H__

#include "Module.h"

#define PHYSFS_PATH "hello"

class ModulePhysFS : public Module
{
public:

	ModulePhysFS();
	~ModulePhysFS();

	void ReadTest();
};

#endif // !__MODULE_PHYSFS_H__