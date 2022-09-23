#include "ModuleXML.h"
#include "QuickSave.h"

ModuleXML::ModuleXML() : Module()
{
	//config = OpenXML(CONFIG_PATH);

	//resource = OpenXML(RESOURCE_PATH);
}

bool ModuleXML::Init()
{
	QuickSave::Init();

	return true;
}

XMLNode ModuleXML::OpenXML(std::string path)
{
	pugi::xml_node n;

	pugi::xml_document* d = new pugi::xml_document();

	XMLNode ret;

	// Check if the document has already been opened
	for (int i = 0; i < xmlFiles.size(); i++)
	{
		if (xmlFiles[i].second == path)
		{
			ret.node = xmlFiles[i].first->first_child();
			ret.xmlFile = i;
			return ret;
		}
	}

	const char* cPath = path.c_str();

	pugi::xml_parse_result result = d->load_file(cPath);
	if (result == NULL)
	{
		LOG("Could not load xml file: %s. pugi error: %s", path.c_str(), result.description());
		return XMLNode();
	}
	else
	{
		//Get first node
		n = d->first_child();
		// Add path and xml_document to the xmlFiles vector
		std::pair<pugi::xml_document*, std::string> p(d, path);
		xmlFiles.push_back(p);
	}

	ret.node = n;
	ret.xmlFile = xmlFiles.size()-1; // Uint that points to the index of the xml_document inside xmlFiles vector.

	return ret;
}

XMLNode ModuleXML::GetConfigXML()
{
	return config;
}

XMLNode ModuleXML::GetResourceXML()
{
	return resource;
}

void ModuleXML::Save(int index)
{
	if (index >= 0)
	{
		xmlFiles[index].first->save_file(xmlFiles[index].second.c_str());
		return;
	}
	
	for (int i = 0; i < xmlFiles.size(); i++)
	{
		xmlFiles[i].first->save_file(xmlFiles[i].second.c_str());
	}
}

bool ModuleXML::CleanUp()
{
	for (int i = 0; i < xmlFiles.size(); i++)
	{
		RELEASE(xmlFiles[i].first);
	}

	xmlFiles.clear();

	return true;
}

