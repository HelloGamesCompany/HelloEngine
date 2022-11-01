#ifndef _MODULE_XML_
#define _MODULE_XML_

#include "Module.h"
#include <vector>

#define CONFIG_DIR "Resources/Editor/Xmls"
#define CONFIG_PATH "Resources/Editor/Xmls/config.xml"
#define RESOURCE_PATH "Resources/Editor/Xmls/resource.xml"

#define KEY 8888

#include "XMLNode.h"

class ModuleXML : public Module
{
public:
	ModuleXML();

	bool Init() override;
	
	/// <summary>
	/// Opens and returns the specified XML. If it already exist, return directly 
	/// </summary>
	/// <param name="path"> : XML path</param>
	/// <returns></returns>
	XMLNode OpenXML(std::string path);

	XMLNode GetConfigXML();

	XMLNode GetResourceXML();

	/// <summary>
	/// Save an xmlDocument using its index
	/// </summary>
	/// <param name="index">-1 = Save all xml_documents</param>
	void Save(int index = -1);

	bool CleanUp() override;

private:
	void CreateDefaultConfigFile();

	/// <summary>
	/// Encrypt or decrypt the gives Xml file
	/// </summary>
	/// <param name="encrypt">: true = encrypt, false =  decrypt</param>
	void EncryptDecryptXML(std::string fileName, bool encrypt);
private:
	XMLNode config;

	XMLNode resource;

	// <xml file, path>
	std::vector<std::pair<pugi::xml_document*, std::string>> xmlFiles;
};

#endif