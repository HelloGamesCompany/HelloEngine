#include "Headers.h"
#include "ModuleXML.h"
#include "QuickSave.h"
#include "ModuleFiles.h"

ModuleXML::ModuleXML() : Module()
{
	if (!ModuleFiles::S_Exists(CONFIG_DIR)) ModuleFiles::S_MakeDir(CONFIG_DIR);

	if (!ModuleFiles::S_Exists(CONFIG_PATH)) CreateDefaultConfigFile();

	config = OpenXML(CONFIG_PATH);

	//resource = OpenXML(RESOURCE_PATH);
}

bool ModuleXML::Init()
{
	QuickSave::Init();

	return true;
}

XMLNode ModuleXML::OpenXML(std::string path)
{
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
	pugi::xml_node n;

	pugi::xml_document* d = new pugi::xml_document();

	EncryptDecryptXML(path, false);

	const char* cPath = path.c_str();

	char* buf = nullptr;

	uint bufSize = ModuleFiles::S_Load(path, &buf);

	// Old manner, 
	//pugi::xml_parse_result result = d->load_file(cPath);

	pugi::xml_parse_result result = d->load_buffer(buf, bufSize);

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

	RELEASE(buf);

	return ret;
}

XMLNode ModuleXML::CreateXML(std::string filePath, std::string rootNodeName)
{
	std::string rootNodeOpen = "<";
	std::string rootNodeClose = "</";
	rootNodeOpen += rootNodeName;
	rootNodeClose += rootNodeName;
	rootNodeOpen += ">\n";
	rootNodeClose += ">\n";

	// Load template Configuration structure into the new file
	std::string context =
		"<?xml version=\"1.0\"?>\n" +
		rootNodeOpen +
		rootNodeClose;

	// Open/create config file
	std::ofstream file(filePath);

	// Override the file with template context
	file << context;

	// Close file
	file.close();

	return OpenXML(filePath);
}

XMLNode ModuleXML::GetConfigXML()
{
	return config;
}

XMLNode ModuleXML::GetResourceXML()
{
	return resource;
}

void ModuleXML::Save(int index, const std::string& extension)
{
	if (index >= 0)
	{
		std::string filePath = xmlFiles[index].second;
		filePath = ModuleFiles::S_RemoveExtension(filePath);
		filePath += extension;
		xmlFiles[index].first->save_file(filePath.c_str());

		//uint size = strlen((char*)xmlFiles[index].first);

		//pugi::xml_writer_file w(xmlFiles[index].first);

		//xmlFiles[index].first->save(w);

		//xmlFiles[index].first->save()

		//char* c = const_cast<char*>(xmlFiles[index].first);

		//uint size = sizeof(c);

		//uint size = sizeof(*xmlFiles[index].first.);

		//char* buffer = xmlFiles[index].first->GetBuffer();

		//uint size=0;

		//for (size = 0; size <700; size++)
		//{
		//	std::cout << buffer[size];
		//}

		//ModuleFiles::S_Save(xmlFiles[index].second.c_str(), buffer, size, false);
		
		//EncryptDecryptXML(xmlFiles[index].second.c_str(), true);
		return;
	}
	
	for (int i = 0; i < xmlFiles.size(); i++)
	{
		xmlFiles[i].first->save_file(xmlFiles[i].second.c_str());
		
		//pugi::xml_writer_file w(xmlFiles[i].first);

		//xmlFiles[index].first->save(w);

		//uint size = sizeof(xmlFiles[i].first);

		/*
		char* buffer = xmlFiles[i].first->GetBuffer();

		uint size = 0;

		for (size = 0; buffer[size] != '\0'; size++) {}

		ModuleFiles::S_Save(xmlFiles[i].second.c_str(), buffer, size, false);
		*/
		//EncryptDecryptXML(xmlFiles[i].second.c_str(), true);
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

void ModuleXML::CreateDefaultConfigFile()
{ 
	LOG("Create default Configuration file");

	// Load template Configuration structure into the new file
	std::string context = 
		"<?xml version=\"1.0\"?>\n"
		"<config>\n"
			"<window>\n"
				"<width value = \"1280\"/>\n"
				"<height value = \"720\"/>\n"
				"<brightness value = \"1\"/>\n"
			"</window>\n"
			"<renderer>\n"
				"<vsync value = \"true\"/>\n"
				"<framerate value = \"90\"/>\n"
			"</renderer>\n"
			"<scene>\n"
				"<currentScene value = \"null\"/>\n"
			"</scene>\n"
			"<openGL>\n"
				"<blend value = \"false\" tag = \"0x0BE2\" />\n"
				"<cullFace value = \"false\" tag = \"0x0B44\" />\n"		
				"<wireframe value = \"false\"/>\n"
			"</openGL>\n"
		"</config>";

	//TODO: Will be subtitute for ModuleFiles functions

	// Open/create config file
	std::ofstream file(CONFIG_PATH);

	// Override the file with template context
	file << context;

	// Close file
	file.close();
}

void ModuleXML::EncryptDecryptXML(std::string fileName, bool encrypt)
{
	bool isEncrypted = false;

	std::ifstream iFile(fileName);

	std::string contex = "";

	std::filebuf* readBuff = iFile.rdbuf();

	char c = readBuff->sbumpc();

	isEncrypted = c != '<' ? true : false;

	// If is already encryped/decryped, then return
	if (isEncrypted == encrypt)
	{
		iFile.close();
		return;
	}

	// Encrypt or decrypt all characters

	for (; c != EOF; c = readBuff->sbumpc()) contex += c ^ KEY;

	iFile.close();

	std::ofstream oFile(fileName);

	oFile << contex;

	oFile.close();
}