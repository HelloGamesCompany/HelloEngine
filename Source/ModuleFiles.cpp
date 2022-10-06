#include "Headers.h"
#include "ModuleFiles.h"
#include "physfs.h"

ModuleFiles::ModuleFiles():Module()
{
	PHYSFS_init(0);

	// Add Write Dir
	if (PHYSFS_setWriteDir(".") == 0) LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	// Add Read Dir
	S_AddPathToFileSystem(".");

	//S_AddPathToFileSystem("Resources");
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

uint ModuleFiles::S_Load(std::string filePath, char** buffer)
{
	uint ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openRead(filePath.c_str());

	if (!fsFile)
	{
		LOG("File System error while opening file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());

		return ret;
	}
	
	PHYSFS_sint64 size = PHYSFS_fileLength(fsFile);

	if (size <= 0)
	{
		LOG("File System error while reading from file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());
		
		return ret;
	}

	*buffer = new char[size + 1];

	ret = (uint)PHYSFS_readBytes(fsFile, *buffer, size);

	if (ret != size)
	{
		LOG("File System error while reading from file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());
		RELEASE_ARRAY(*buffer);
		return ret;
	}
	else
	{
		//Adding end of file at the end of the buffer. Loading a shader file does not add this for some reason
		(*buffer)[size] = '\0';
	}

	if (PHYSFS_close(fsFile) == 0) LOG("File System error while closing file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());
		
	return ret;
}

uint ModuleFiles::S_Save(std::string filePath, char* buffer, uint size, bool append)
{
	uint byteCount = 0;

	bool exist = S_Exists(filePath);

	PHYSFS_file* des = nullptr;

	if (append)	des = PHYSFS_openAppend(filePath.c_str());
	else des = PHYSFS_openWrite(filePath.c_str());

	do 
	{
		if (!des)
		{
			LOG("FILE SYSTEM: Could not open file '%s' to write. ERROR: %s", filePath.c_str(), PHYSFS_getLastError());

			break;
		}

		byteCount = PHYSFS_writeBytes(des, (const void*)buffer, size);

		if (byteCount != size)
		{
			LOG("FILE SYSTEM: Could not write to file '%s'. ERROR: %s", filePath.c_str(), PHYSFS_getLastError());

			break;
		}

		if(!exist) 
		{
			LOG("FILE SYSTEM: New file '%s' created with %u bytes", filePath.c_str(), byteCount);
			
			break;
		}

		if (append)
		{
			LOG("FILE SYSTEM: Append %u bytes to file '%s'", byteCount, filePath.c_str());

			break;
		}
		
		LOG("FILE SYSTEM: File '%s' overwritten with %u bytes", filePath.c_str(), byteCount);

	} while (false);

	if (PHYSFS_close(des) == 0) LOG("FILE SYSTEM: Could not close file '%s'. ERROR: %s", filePath.c_str(), PHYSFS_getLastError());
	
	return byteCount;
}