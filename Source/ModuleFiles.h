#ifndef __MODULE_PHYSFS_H__
#define __MODULE_PHYSFS_H__

#include "Module.h"

class FileTree;
class Directory;

#define ASSETS_PATH "Assets/"
#define ASSETS_NAME "Assets"
#define RESOURCE_PATH "Resources/"
#define RESOURCE_NAME "Resources"

enum class ResourceType
{
	UNDEFINED,
	MESH,
	TEXTURE,
	MODEL,
	SCENE,
};

struct MetaFile
{
	ResourceType type = ResourceType::UNDEFINED;
	std::string resourcePath;
	uint UID;
	time_t lastModified;
	std::string name = "Null";
};

class ModuleFiles : public Module
{
public:
	ModuleFiles();

	~ModuleFiles();

	// Static functions
	static bool S_Exists(const std::string& file);

	static bool S_CheckFinishWith(const std::string& file, const std::string& checker);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="dir"> : Path with name</param>
	/// <returns></returns>
	static bool S_MakeDir(const std::string& dir);

	static bool S_IsDirectory(const std::string& file);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="file"> :$(SolutionDir)Output/relative path</param>
	/// <returns></returns>
	static bool S_Delete(const std::string& file);

	/*static std::string S_GlobalToLocalPath(const std::string path);*/

	static std::string S_NormalizePath(const std::string& path);

	static std::string S_UnNormalizePath(const std::string& path);

	// can be path or zip
	static bool S_AddPathToFileSystem(const std::string& path);

	/// <summary>
	/// Read directory => /Output
	/// </summary>
	/// <param name="filePath">: path/file.ext</param>
	/// <param name="buffer">: buffer where store binary file info</param>
	/// <returns></returns>
	static uint S_Load(const std::string& filePath, char** buffer);

	static MetaFile S_LoadMeta(const std::string& filePath);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="filePath">: $(SolutionDir)Output/relative path </param>
	/// <param name="buffer"></param>
	/// <param name="size"></param>
	/// <param name="append"></param>
	/// <returns></returns>
	static uint S_Save(const std::string& filePath, char* buffer, uint size, bool append);

	/// <summary>
	/// Use for copy project file -> project folder
	/// </summary>
	/// <param name="src">: PATH WITH NAME!!!</param>
	/// <param name="des">: JUST PATH!!!</param>
	/// <param name="replace">: Replace file if is aldready exist</param>
	/// <returns></returns>
	static bool S_Copy(const std::string& src, std::string des, bool replace = true);

	/// <summary>
	/// Use for copy external file -> project folder.
	/// </summary>
	/// <param name="src">: Should be global path + name</param>
	/// <param name="des">: Should be local path</param>
	/// <param name="replace">: Replace file if is aldready exist</param>
	/// <returns></returns>
	static bool S_ExternalCopy(const std::string& src, std::string des, bool replace = true);

	/// <summary>
	/// Root path = Assets/
	/// </summary>
	/// <param name="fileTree"></param>
	static void S_UpdateFileTree(FileTree*& fileTree);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="path">: $(SolutionDir)Output/relative path </param>
	/// <returns></returns>
	static unsigned long long S_CheckFileLastModify(const std::string& path);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="path"> Relative path </param>
	static void S_OpenFolder(const std::string& path);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="getExtention">: If you want to get the extension</param>
	/// <returns></returns>
	static std::string S_GetFileName(const std::string& file, bool getExtension = true);
	
	static std::string S_GetFileExtension(const std::string& file);

	static std::string S_RemoveExtension(const std::string& file);

	static std::string S_FilePath(const std::string& file);

	static ResourceType S_GetResourceType(const std::string& filename);

	static bool S_CheckMetaExist(const std::string& file);

	static bool S_CreateMetaData(const std::string& file, const std::string& resourcePath);

	static bool S_UpdateMetaData(const std::string& file, const std::string& resourcePath);

private:

	/// <summary>
	/// Internal Function, do not use it!!!
	/// ----- You maybe look for S_UpdateFileTree()
	/// </summary>
	static bool UpdateFileNodeRecursive(Directory*& dir, Directory*& lastDir);

	static void DeleteDirectoryRecursive(std::string directory);
};

#endif // !__MODULE_PHYSFS_H__