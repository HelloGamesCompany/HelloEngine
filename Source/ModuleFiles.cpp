#include "Headers.h"
#include "ModuleFiles.h"
#include "physfs.h"

ModuleFiles::ModuleFiles():Module()
{
	PHYSFS_init(0);
}

ModuleFiles::~ModuleFiles()
{
	PHYSFS_deinit();
}

void ModuleFiles::ReadTest()
{
	PHYSFS_file* file = PHYSFS_openRead(PHYSFS_PATH);

	if (file == NULL)
	{
		std::cout << "load file failed: " << PHYSFS_getLastError() << std::endl;
			
	}
	else std::cout << "open file with successful" << std::endl;

	PHYSFS_close(file);
}