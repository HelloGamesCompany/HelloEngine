#include "Headers.h"
#include "ModulePhysFS.h"
#include "physfs.h"

ModulePhysFS::ModulePhysFS():Module()
{
	PHYSFS_init(0);
}

ModulePhysFS::~ModulePhysFS()
{
	PHYSFS_deinit();
}

void ModulePhysFS::ReadTest()
{
	PHYSFS_file* file = PHYSFS_openRead(PHYSFS_PATH);

	if (file == NULL)
	{
		std::cout << "load file failed: " << PHYSFS_getLastError() << std::endl;
			
	}
	else std::cout << "open file with successful" << std::endl;

	PHYSFS_close(file);
}