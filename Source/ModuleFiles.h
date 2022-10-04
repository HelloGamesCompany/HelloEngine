#ifndef __MODULE_PHYSFS_H__
#define __MODULE_PHYSFS_H__

#include "Module.h"

class ModuleFiles : public Module
{
public:
	ModuleFiles();

	~ModuleFiles();

	// Static functions
	static bool S_Exists(const std::string file);

	static bool S_MakeDir(const std::string dir);

	static bool S_IsDirectory(const std::string file);

	// can be path or zip
	static bool S_AddPathToFileSystem(const std::string path);

	/// <summary>
	/// woking dir = Output/Assets
	/// </summary>
	/// <param name="filePath">: path/file.ext</param>
	/// <returns> binary information </returns>
	static char* S_Load(std::string filePath);
};

#endif // !__MODULE_PHYSFS_H__