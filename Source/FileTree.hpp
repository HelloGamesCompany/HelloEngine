#pragma once

#include <string>
#include <vector>

class FileTree
{
public:
	FileTree() {};

	FileTree(std::string path) { this->path = path; }

	FileTree(std::string path, std::string name)
	{
		this->path = path; 
		this->name = name;
	}

	std::vector<FileTree> directories;

	std::vector<std::string> files;

	std::string path;

	std::string name;
};