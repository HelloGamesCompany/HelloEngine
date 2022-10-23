#ifndef __MODULE_PHYSFS_H__
#define __MODULE_PHYSFS_H__

#include "Module.h"

class FileTree;

enum class ResourceType
{
	UNDEFINED,
	MESH,
	TEXTURE
};

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
	/// Read directory => /Output
	/// </summary>
	/// <param name="filePath">: path/file.ext</param>
	/// <param name="buffer">: buffer where store binary file info</param>
	/// <returns></returns>
	static uint S_Load(const std::string filePath, char** buffer);

	static uint S_Save(const std::string filePath, char* buffer, uint size, bool append);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="src">: PATH WITH NAME!!!</param>
	/// <param name="des">: JUST PATH!!!</param>
	/// <param name="replace">: Replace file if is aldready exist</param>
	/// <returns></returns>
	static uint S_Copy(const std::string src, const std::string des, bool replace = true);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="path">: path will be start in folder Output</param>
	/// <returns></returns>
	static FileTree S_GetFileTree(std::string path);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="file"></param>
	/// <param name="getExtention">: If you want to get the extension</param>
	/// <returns></returns>
	static std::string S_GetFileName(const std::string file, bool getExtension = true);

	static ResourceType S_GetResourceType(const std::string& filename);
};

#endif // !__MODULE_PHYSFS_H__