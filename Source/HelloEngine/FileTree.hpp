#pragma once

#include <string>
#include <vector>

#include "Application.h"
#include "ModuleResourceManager.h"

struct Directory;

struct File
{
	File() {};

	File(std::string path, std::string name, Directory* parent) :path(path), name(name), parent(parent)
	{
		// Check if this file has a .helloMeta file with the same name.
		if (ModuleFiles::S_CheckMetaExist(path))
		{
			metaPath = path + ".helloMeta";
			metaFile = ModuleFiles::S_LoadMeta(metaPath);

			// Check if this meta file has a different modify time than the file.
			unsigned long long modifyTime = ModuleFiles::S_CheckFileLastModify(path);
			if (metaFile.lastModified != modifyTime)
			{
				if (ModuleResourceManager::resources.count(metaFile.UID) != 0)
					ModuleResourceManager::S_ReImportFile(path, metaFile.type);
				else
				{
					ModuleResourceManager::S_CreateResource(metaFile);
					ModuleResourceManager::S_ReImportFile(path, metaFile.type);
				}

				metaFile = ModuleFiles::S_LoadMeta(metaPath);
			}
		}
		else
		{
			// If the resource type of this file is not undefined:
			// iF it doesn't have a meta file, create one by using the ImportFile method inside ModuleResourceManager
			if (ModuleFiles::S_GetResourceType(path) == ResourceType::UNDEFINED ||
				ModuleFiles::S_GetResourceType(path) == ResourceType::SCENE)
				return;

			ModuleResourceManager::S_ImportFile(path);

			metaPath = path + ".helloMeta";
			metaFile = ModuleFiles::S_LoadMeta(metaPath);

			ModuleResourceManager::S_CreateResource(metaFile);

		}
	};
	std::string path;
	std::string metaPath = "none";
	std::string name;
	Directory* parent = nullptr;
	MetaFile metaFile;
	bool pressed = false;
};

struct Directory
{
	Directory(std::string path, std::string name, Directory* parent) :path(path), name(name), parent(parent) {};
	~Directory()
	{
		for (size_t i = 0; i < directories.size(); i++)
		{
			RELEASE(directories[i]);
		}
	}
	std::vector<Directory*> directories;
	std::vector<File> files;
	std::string path;
	std::string name;
	Directory* parent = nullptr;
};

class FileTree
{
public:
	FileTree() {};

	~FileTree()
	{
		RELEASE(_root);
	}

	bool GetRootDir(Directory*& dir)
	{
		if (_root)
		{
			dir = _root;
			true;
		}
		return false;
	}

	void SetNewRoot(Directory* root)
	{
		if (_root)
		{
			RELEASE(_root);
		}
		_root = root;
	}

	std::string GetRootPath()
	{
		return _rootPath;
	}

public:
	Directory* _currentDir = nullptr;

private:
	Directory* _root = nullptr;

	std::string _rootPath;
};
