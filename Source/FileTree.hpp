#pragma once

#include <string>
#include <vector>

#include "ModuleFiles.h"

struct Directory;

struct File
{
	File(std::string path, std::string name, Directory* parent) :path(path), name(name), parent(parent)
	{
		// Check if this file has a .helloMeta file with the same name.
		if (ModuleFiles::S_CheckMetaExist(path))
		{
			std::string metapath = path + ".helloMeta";
			// Check if this meta file has a different modify time than the file.
			metaFile = ModuleFiles::S_LoadMeta(metapath);
		}
		else
		{
			// If the resource type of this file is not undefined:
			// iF it doesn't have a meta file, create one by using the ImportFile method inside ModuleResourceManager

		}

		// If it does have a meta file, check if the change date is newer than importing date.
		// if the change date is newer than the import date, call import of moduleResourcemanager again (reimport file)
		// else, we can continue.
	};
	std::string path;
	std::string name;
	Directory* parent = nullptr;
	MetaFile metaFile;
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