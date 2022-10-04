#include "Headers.h"
#include "ModuleFiles.h"
#include "physfs.h"

ModuleFiles::ModuleFiles():Module()
{
	PHYSFS_init(0);

	if (PHYSFS_setWriteDir("./Assets") == 0) LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	//S_AddPathToFileSystem(".");
	//S_AddPathToFileSystem("Assets");

	S_MakeDir("hi");
}

ModuleFiles::~ModuleFiles()
{
	PHYSFS_deinit();
}

bool ModuleFiles::S_Exists(const std::string file)
{
	return PHYSFS_exists(file.c_str()) != 0;
}

bool ModuleFiles::S_MakeDir(const std::string dir)
{
	if (S_IsDirectory(dir) == false)
	{
		PHYSFS_mkdir(dir.c_str());
		return true;
	}
	return false;
}

bool ModuleFiles::S_IsDirectory(const std::string file)
{
	return PHYSFS_isDirectory(file.c_str()) != 0;
}

bool ModuleFiles::S_AddPathToFileSystem(const std::string path)
{
	bool ret = false;

	if (PHYSFS_mount(path.c_str(), nullptr, 1) == 0)
	{
		LOG("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	}
	else
		ret = true;

	return ret;
}

char* ModuleFiles::S_Load(std::string filePath)
{
	char* buffer = 0;

	PHYSFS_file* fsFile = PHYSFS_openRead(filePath.c_str());

	if (!fsFile)
	{
		LOG("File System error while opening file %s: %s\n", filePath, PHYSFS_getLastError());

		return nullptr;
	}
	
	PHYSFS_sint64 size = PHYSFS_fileLength(fsFile);

	if (size <= 0)
	{
		LOG("File System error while reading from file %s: %s\n", filePath, PHYSFS_getLastError());
		
		return nullptr;
	}

	buffer = new char[size + 1];

	if (PHYSFS_readBytes(fsFile, buffer, size) != size)
	{
		LOG("File System error while reading from file %s: %s\n", filePath, PHYSFS_getLastError());
		RELEASE_ARRAY(buffer);
		return nullptr;
	}
	else
	{
		//Adding end of file at the end of the buffer. Loading a shader file does not add this for some reason
		buffer[size] = '\0';
	}

	if (PHYSFS_close(fsFile) == 0) LOG("File System error while closing file %s: %s\n", filePath, PHYSFS_getLastError());
		
	return buffer;
}