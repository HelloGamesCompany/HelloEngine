#include "Headers.h"
#include "ModuleFiles.h"
#include "physfs.h"
#include "FileTree.hpp"
#include <algorithm>
#include <ctime>
#include "Console.h"
#include "json.hpp"

using json = nlohmann::json;

ModuleFiles::ModuleFiles():Module()
{	
	Console::S_Init();

	Console::S_Log("Initializing PhysFS.");

	PHYSFS_init(0);

	// Add Write Dir
	if (PHYSFS_setWriteDir(".") == 0) LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	// Add Read Dir
	S_AddPathToFileSystem(".");

	//S_AddPathToFileSystem("Resources");
}

ModuleFiles::~ModuleFiles()
{
	Console::S_Close();

	PHYSFS_deinit();
}

bool ModuleFiles::S_Exists(const std::string& file)
{
	return PHYSFS_exists(file.c_str()) != 0;
}

bool ModuleFiles::S_MakeDir(const std::string& dir)
{
	if (S_IsDirectory(dir) == false)
	{
		PHYSFS_mkdir(dir.c_str());
		return true;
	}
	return false;
}

bool ModuleFiles::S_IsDirectory(const std::string& file)
{
	PHYSFS_Stat fileState;
	PHYSFS_stat(file.c_str(), &fileState);

	return fileState.filetype == PHYSFS_FileType::PHYSFS_FILETYPE_DIRECTORY;
}

bool ModuleFiles::S_Delete(const std::string& file)
{
	return PHYSFS_delete(file.c_str());
}

/*
std::string ModuleFiles::S_GlobalToLocalPath(const std::string path)
{
	std::string localPath = S_NormalizePath(path);

	size_t pos = 0;

	pos = localPath.find(ASSETS_PATH);

	if (pos != std::string::npos)
	{
		localPath = localPath.substr(pos);

		if (localPath.c_str() != "") return localPath;
	}

	return localPath;
}
*/

std::string ModuleFiles::S_NormalizePath(const std::string& path)
{
	std::string ret = path;

	for (int i = 0; i < ret.size(); i++)
	{
		if (ret[i] == '\\')
		{
			ret[i] = '/';
		}
	}

	return ret;
}

std::string ModuleFiles::S_UnNormalizePath(const std::string& path)
{
	std::string ret = path;

	for (int i = 0; i < ret.size(); i++)
	{
		if (ret[i] == '/') ret[i] = '\\';
	}

	return ret;
}

bool ModuleFiles::S_AddPathToFileSystem(const std::string& path)
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

uint ModuleFiles::S_Load(const std::string& filePath, char** buffer)
{
	uint byteCount = 0;

	PHYSFS_file* fsFile = PHYSFS_openRead(filePath.c_str());

	do 
	{
		if (!fsFile)
		{
			LOG("File System error while opening file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());

			break;
		}

		PHYSFS_sint64 size = PHYSFS_fileLength(fsFile);

		if (size <= 0)
		{
			LOG("File System error while reading from file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());

			break;
		}

		*buffer = new char[size + 1];

		byteCount = (uint)PHYSFS_readBytes(fsFile, *buffer, size);

		if (byteCount != size)
		{
			LOG("File System error while reading from file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());
			RELEASE_ARRAY(*buffer);
			break;
		}
		
		//Adding end of file at the end of the buffer. Loading a shader file does not add this for some reason
		(*buffer)[size] = '\0';
	
	} while (false);
	
	if (PHYSFS_close(fsFile) == 0)
	{
		LOG("File System error while closing file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());
	}

	return byteCount;
}

MetaFile ModuleFiles::S_LoadMeta(const std::string& filePath)
{
	char* data = nullptr;
	S_Load(filePath, &data);
	json file = json::parse(data);

	MetaFile ret;
	ret.lastModified = file["Last modify"];
	ret.resourcePath = file["Resource path"];
	ret.type = file["Resource type"];
	ret.UID = file["UID"];
	ret.name = file["Name"];

	RELEASE(data);

	return ret;
}

uint ModuleFiles::S_Save(const std::string& filePath, char* buffer, uint size, bool append)
{
	uint byteCount = 0;

	bool exist = S_Exists(filePath);

	PHYSFS_file* des = nullptr;

	do 
	{
		if (append)	des = PHYSFS_openAppend(filePath.c_str());
		else des = PHYSFS_openWrite(filePath.c_str());

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

	if (PHYSFS_close(des) == 0)
	{
		LOG("FILE SYSTEM: Could not close file '%s'. ERROR: %s", filePath.c_str(), PHYSFS_getLastError());
	}

	return byteCount;
}

bool ModuleFiles::S_Copy(const std::string& src, std::string des, bool replace)
{
	bool successful = true;

	bool isDir = S_IsDirectory(src);

	// Files case
	if (!isDir) 
	{
		std::string fileName = S_GetFileName(src, true);

		des += fileName;

		char* buffer = nullptr;

		do
		{
			if (S_Exists(des) && !replace)
			{
				LOG("FILE SYSTEM: the file you want to copy is already exist and you don't want to replace this: '%s'", src.c_str());
				successful = false;
				break;
			}

			uint srcSize = S_Load(src, &buffer);

			if (srcSize <= 0)
			{
				if (S_Exists(src))
				{
					S_Save(des, buffer, srcSize, false);
					break;
				}

				LOG("FILE SYSTEM: Could not read from file '%s'", src.c_str());
				successful = false;
				break;
			}

			uint desSize = S_Save(des, buffer, srcSize, false);

			if (desSize <= 0)
			{
				LOG("FILE SYSTEM: Could not save file '%s'", src.c_str());
				successful = false;
				break;
			}

			LOG("FILE SYSTEM: Successfully copied source file: '%s' to the destination file: '%s'", src.c_str(), des.c_str());

		} while (false);

		RELEASE(buffer);
	}
	// Folder case
	else
	{
		// TODO: Copy entire folder
	}

	return successful;
}

bool ModuleFiles::S_ExternalCopy(const std::string& src, std::string des, bool replace)
{
	 std::string workingDir = S_NormalizePath(std::filesystem::current_path().string());

	 // Change destination file to correspondent formmat
	 if (des[0] != '/')
	 {
		 des.insert(des.begin(), '/');
	 }

	des = workingDir + des + S_GetFileName(src);

	std::ifstream srcFile(src, std::ios::binary);
	if(srcFile.is_open() == 0)
	{
		LOG("Faild to oppen src file");
		Console::S_Log("Faild to oppen src file");
		srcFile.close();
		return false;
	}

	std::ofstream desFile(des, std::ios::binary);
	if (desFile.is_open() == 0)
	{
		LOG("Faild to create/oppend destination file");
		Console::S_Log("Faild to create/oppend destination file");
		desFile.close();
		return false;
	}

	// copy context
	desFile << srcFile.rdbuf();

	srcFile.close();

	desFile.close();

	return true;
}

void ModuleFiles::S_UpdateFileTree(FileTree*& fileTree)
{
	// Init Root
	Directory* root = new Directory(ASSETS_PATH, ASSETS_NAME, nullptr);
	// Get FileTree last directory
	Directory* LastDir = fileTree->_currentDir;

	bool hasLastDir = UpdateFileNode(root, LastDir);
	if (!hasLastDir)
	{
		fileTree->_currentDir = root;
	}
	else
	{
		fileTree->_currentDir = LastDir;
	}
	fileTree->SetNewRoot(root);
}

bool ModuleFiles::UpdateFileNode(Directory*& dir, Directory*& lastDir)
{
	// Check if lastDir still exist
	bool ret = false;

	// Get all files
	char** fileList = PHYSFS_enumerateFiles(dir->path.c_str());

	for (int i = 0; fileList[i] != nullptr; i++)
	{
		std::string dirCheck = dir->path + fileList[i];
		// File case
		if (!S_IsDirectory(dirCheck))
		{
			// We shouldn't care metas. 
			if(S_GetFileExtension(S_GetFileName(dirCheck)) == "hellometa")
			{
				// Check if this meta has an associated file.
				std::string fileDir = dirCheck.substr(0, dirCheck.find_last_of(".")+1);

				if (!S_Exists(fileDir))
					ModuleResourceManager::S_DeleteMetaFile(dirCheck);
				else
					ModuleResourceManager::S_CreateResource(S_LoadMeta(dirCheck));

				continue;
			}

			// Change directory construcotr to create meta data if necessary
			dir->files.emplace_back(dirCheck, S_GetFileName(dirCheck), dir);

			continue;
		}

		dirCheck += "/"; // Add '/' if this is a directory.

		// Folder case
		Directory* currentDir = new Directory(dirCheck, S_GetFileName(dirCheck), dir);
		dir->directories.push_back(currentDir);

		// Check if still exist the last Directory we opened
		// If we have already found this
		if (ret)
		{
			UpdateFileNode(currentDir, lastDir);
			continue;
		}

		// If we still haven't found, we check if current dir is we wanted
		if (!lastDir || lastDir->path != currentDir->path)
		{
			ret = UpdateFileNode(currentDir, lastDir);
			continue;
		}

		ret = true;
		lastDir = currentDir;
		UpdateFileNode(currentDir, lastDir);
	}
	return ret;
}

unsigned long long ModuleFiles::S_CheckFileLastModify(const std::string& path)
{
	PHYSFS_Stat fileState;
	if (PHYSFS_stat(path.c_str(), &fileState) == 0)
	{
		LOG("Failed to open file in S_CheckFileLastModify!");
		return 0;
	}

	return fileState.modtime;
}

std::string ModuleFiles::S_GetFileName(const std::string& file, bool getExtension)
{
	uint pos = file.find_last_of("/");
	std::string ret = file;

	if (pos != std::string::npos && file[pos] == file.back())
	{
		ret = file.substr(0, pos);
		pos = ret.find_last_of("/");
	}

	if (pos != std::string::npos)
	{
		ret = ret.substr(pos + 1, file.size() - 1);
	}
	else
	{
		ret = file;
	}

	if (!getExtension)
	{
		ret = S_RemoveExtension(ret);
	}

	return ret;
}

std::string ModuleFiles::S_GetFileExtension(const std::string& file)
{
	std::string ret = file;

	ret = ret.substr(ret.find_last_of('.') + 1);

	std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);

	return ret;
}

std::string ModuleFiles::S_RemoveExtension(const std::string& file)
{
	uint pos = file.find_last_of(".");

	std::string ret = file;

	if (pos != std::string::npos)
	{
		ret = file.substr(0, pos);
	}

	return ret;
}

std::string ModuleFiles::S_FilePath(const std::string& file)
{
	return "In process...";
}

ResourceType ModuleFiles::S_GetResourceType(const std::string& filename)
{
	std::string fileExtension = S_GetFileExtension(filename);

	//TODO: Add our own file extensions to this checks

	if (fileExtension == "fbx" || fileExtension == "dae") return ResourceType::MODEL;
	if (fileExtension == "tga" || fileExtension == "png" || fileExtension == "jpg" || fileExtension == "dds") return ResourceType::TEXTURE;

	return ResourceType::UNDEFINED;
}

bool ModuleFiles::S_CheckMetaExist(const std::string& file)
{
	std::string meta = file + ".helloMeta";

	return S_Exists(meta);
}

bool ModuleFiles::S_CreateMetaData(const std::string& file, const std::string& resourcePath)
{
	std::string newFile = file + ".helloMeta";
	std::string assetName = ModuleFiles::S_GetFileName(file);
	// Create json object
	json j;

	// Get modify time
	time_t currentTime = S_CheckFileLastModify(file);

	// Update json values
	j["Last modify"] = currentTime;

	j["Resource path"] = resourcePath;

	j["Resource type"] = ModuleFiles::S_GetResourceType(file);

	j["UID"] = HelloUUID::GenerateUUID();

	j["Name"] = assetName;

	// write to string
	std::string meta = j.dump();

	ModuleFiles::S_Save(newFile, &meta[0], meta.size(), false);

	return true;

}

bool ModuleFiles::S_UpdateMetaData(const std::string& file, const std::string& resourcePath)
{
	std::string metaFile = file + ".helloMeta";

	// Destroy resources attached to this meta file
	ModuleResourceManager::S_DeleteMetaFile(metaFile, true);

	char* data = nullptr;
	ModuleFiles::S_Load(metaFile, &data);

	// Create json object
	json j = json::parse(data);

	//// Get modify time
	time_t currentTime = S_CheckFileLastModify(file);

	// Update json values
	j["Last modify"] = currentTime;

	j["Resource path"] = resourcePath;

	// write to string
	std::string meta = j.dump();

	ModuleFiles::S_Save(metaFile, &meta[0], meta.size(), false);

	RELEASE(data);

	return true;
}

