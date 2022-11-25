#pragma once

#include <string>
#include <vector>

using namespace std;

struct Directory;

struct File
{
	File(string path, string name, Directory* parent) :path(path), name(name), parent(parent)
	{
	};
	string path;
	string name;
	Directory* parent = nullptr;
};

struct Directory
{
	Directory(string path, string name, Directory* parent) :path(path), name(name), parent(parent) {};
	~Directory()
	{
		for (size_t i = 0; i < directories.size(); i++)
		{
			RELEASE(directories[i]);
		}
	}
	vector<Directory*> directories;
	vector<File> files;
	string path;
	string name;
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

	string GetRootPath()
	{
		return _rootPath;
	}

public:
	Directory* _currentDir = nullptr;

private:
	Directory* _root = nullptr;

	string _rootPath;
};