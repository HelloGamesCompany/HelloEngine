#ifndef _MODULE_XML_
#define _MODULE_XML_

#include "Module.h"
#include <vector>

#define CONFIG_PATH "XMLs/config.xml"
#define RESOURCE_PATH "XMLs/resource.xml"

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
	XMLNode config;

	XMLNode resource;

	std::vector<std::pair<pugi::xml_document*, std::string>> xmlFiles;
};

#endif

