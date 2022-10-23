#pragma once
#include "Module.h"

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager();
	~ModuleResourceManager();

	/// <summary>
	/// Import a file from assets to our desired file format.
	/// </summary>
	void ImportFile(const std::string& filePath);

private:
};

