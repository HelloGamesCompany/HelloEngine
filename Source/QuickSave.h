#ifndef _QUICK_SAVE_
#define _QUICK_SAVE_

#include "pugixml.hpp"
#include "ModuleXML.h"

#define QUICKSAVE_PATH "Resources/Xmls/quickSave.xml"

// static class
class QuickSave
{
public:
	static void Init();

	static void SetBool(std::string name, bool value);
	static void SetString(std::string name, std::string value);
	static void SetFloat(std::string name, float value);
	static void SetInt(std::string name, int value);

	static bool GetBool(std::string name, bool defaultValue = false);
	static std::string GetString(std::string name, std::string defaultValue = "DefaultValue");
	static float GetFloat(std::string name, float defaultValue = 0);
	static int GetInt(std::string name, int defaultValue = 0);

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