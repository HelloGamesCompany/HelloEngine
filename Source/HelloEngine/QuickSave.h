#ifndef _QUICK_SAVE_
#define _QUICK_SAVE_

#include "pugixml.hpp"
#include "ModuleXML.h"

#define QUICKSAVE_PATH "Resources/Editor/Xmls/quickSave.xml"

// static class
class QuickSave
{
public:
	static void Init();

	// Set Values
	static void SetBool(std::string name, bool value);
	static void SetString(std::string name, std::string value);
	static void SetFloat(std::string name, float value);
	static void SetInt(std::string name, int value);

	static void SetBool(const char* name, bool value);
	static void SetString(const char* name, const char* value);
	static void SetFloat(const char* name, float value);
	static void SetInt(const char* name, int value);

	// Get Values
	static bool GetBool(std::string name, bool defaultValue = false);
	static std::string GetString(std::string name, std::string defaultValue = "DefaultValue");
	static float GetFloat(std::string name, float defaultValue = 0);
	static int GetInt(std::string name, int defaultValue = 0);

	static bool GetBool(const char* name, bool defaultValue = false);
	static const char* GetString(const char* name, const char* defaultValue = "DefaultValue");
	static float GetFloat(const char* name, float defaultValue = 0);
	static int GetInt(const char* name, int defaultValue = 0);

	// Get root of Nodes, just for engine develop use.
	static XMLNode GetRootNodeBool();
	static XMLNode GetRootNodeString();
	static XMLNode GetRootNodeFloat();
	static XMLNode GetRootNodeInt();

	// For reset nodes.
	static void ResetAllNodes();
	static void ResetBoolNode();
	static void ResetFloatNode();
	static void ResetIntNode();
	static void ResetStringNode();

private:
	static void SetValue(std::string name, std::string value, XMLNode& node);

	static std::string GetValue(std::string name, std::string value, XMLNode& node);

	static void CreateQuickSaveDefaultFile();

private:
	static XMLNode nBool;
	static XMLNode nString;
	static XMLNode nFloat;
	static XMLNode nInt;
};

#endif