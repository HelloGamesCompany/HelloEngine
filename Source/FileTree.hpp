#pragma once

#include <string>
#include <vector>

class FileTree
{
public:
	FileTree() {};

	FileTree(std::string path, FileTree* parent) 
	{
		this->path = path; 
		this->_parent = parent;
	}

	FileTree(std::string path, std::string name, FileTree* parent)
	{
		this->path = path; 
		this->name = name;
		this->_parent = parent;
	}

	~FileTree()
	{
		for (int i = 0; i < directories.size(); i++)
		{
			RELEASE(directories[i]);
		}
	}

	FileTree* GetRoot() 
	{
		if (_parent) return _parent->GetRoot();

		return this;
	}

	FileTree* GetParent()
	{
		return _parent;
	}

	bool isRoot() { return _parent ? false : true; }

	std::vector<FileTree*> directories;

	std::vector<std::string> files;

	// Start without "/"
	std::string path;

	std::string name;

private:
	FileTree* _parent = nullptr;
};