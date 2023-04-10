#include "Headers.h"
#include "QuickSave.h"
#include "ModuleXML.h"
#include "ModuleFiles.h"

XMLNode QuickSave::nBool;
XMLNode QuickSave::nString;
XMLNode QuickSave::nFloat;
XMLNode QuickSave::nInt;

void QuickSave::Init()
{
    LOG("Init QuickSave");

    // Create The XML file while is not exist
    if (!ModuleFiles::S_Exists(QUICKSAVE_PATH)) 
        CreateQuickSaveDefaultFile();

    XMLNode node = Application::Instance()->xml->OpenXML(QUICKSAVE_PATH);

    // For align the xml file
    node.Save();

    nBool = node.FindChildBreadth("Bool");
    nString = node.FindChildBreadth("String");
    nFloat = node.FindChildBreadth("Float");
    nInt = node.FindChildBreadth("Int");
}

void QuickSave::SetBool(std::string name, bool value)
{
    SetValue(name, (value ? "true" : "false"), nBool);
}

void QuickSave::SetString(std::string name, std::string value)
{
    SetValue(name, value, nString);
}

void QuickSave::SetFloat(std::string name, float value)
{
    SetValue(name, std::to_string(value)+"f", nFloat);
}

void QuickSave::SetInt(std::string name, int value)
{
    SetValue(name, std::to_string(value), nInt);
}

bool QuickSave::GetBool(std::string name, bool defaultValue)
{
    std::string value = GetValue(name, std::to_string(defaultValue), nBool);
    bool bValue = value == "true";
    return bValue;
}

std::string QuickSave::GetString(std::string name, std::string defaultValue)
{
    std::string value = GetValue(name, defaultValue, nString);
    return value;
}

float QuickSave::GetFloat(std::string name, float defaultValue)
{
    std::string value = GetValue(name, std::to_string(defaultValue), nFloat);
    return std::stof(value);
}

int QuickSave::GetInt(std::string name, int defaultValue)
{
    std::string value = GetValue(name, std::to_string(defaultValue), nInt);
    return std::stoi(value);
}

void QuickSave::SetBool(const char* name, bool value)
{
    SetValue(name, (value ? "true" : "false"), nBool);
}

void QuickSave::SetString(const char* name, const char* value)
{
    SetValue(name, value, nString);
}

void QuickSave::SetFloat(const char* name, float value)
{
    SetValue(name, std::to_string(value) + "f", nFloat);
}

void QuickSave::SetInt(const char* name, int value)
{
    SetValue(name, std::to_string(value), nInt);
}

bool QuickSave::GetBool(const char* name, bool defaultValue)
{
    std::string value = GetValue(name, std::to_string(defaultValue), nBool);

    return (value == "true");
}

const char* QuickSave::GetString(const char* name, const char* defaultValue)
{
    std::string value = GetValue(name, defaultValue, nString);
    size_t length = value.length() + 1;
    char* str = new char[length];
    strcpy_s(str, length, value.c_str());

    return str;
}

float QuickSave::GetFloat(const char* name, float defaultValue)
{
    std::string value = GetValue(name, std::to_string(defaultValue), nFloat);
    return std::stof(value);
}

int QuickSave::GetInt(const char* name, int defaultValue)
{
    std::string value = GetValue(name, std::to_string(defaultValue), nInt);
    return std::stoi(value);
}

XMLNode QuickSave::GetRootNodeBool()
{
    return nBool;
}

XMLNode QuickSave::GetRootNodeString()
{
    return nString;
}

XMLNode QuickSave::GetRootNodeFloat()
{
    return nFloat;
}

XMLNode QuickSave::GetRootNodeInt()
{
    return nInt;
}

void QuickSave::SetValue(std::string name, std::string value, XMLNode& node)
{
    XMLNode n = node.FindChildBreadth(name);

    // If there wasn't any node with the given name, create one.
    if (n == node)
    {
        pugi::xml_node newNode = n.node.append_child(name.c_str());
        pugi::xml_attribute att = newNode.append_attribute("value");

        att.set_value(value.c_str());
    }
    else n.node.attribute("value").set_value(value.c_str());

    n.Save();
}

std::string QuickSave::GetValue(std::string name, std::string value, XMLNode& node)
{
     XMLNode n = node.FindChildBreadth(name);
     std::string ret = value;

    // If there wasnt any node with the given name, return default value.
    if (n != node) 
        ret = n.node.attribute("value").as_string();
    
    return ret;
}

void QuickSave::CreateQuickSaveDefaultFile()
{
    LOG("Create default QuickSave file");

    // Load template QuickSave structure into the new file
    std::string context = 
        "<?xml version = \"1.0\"?>\n"
        "<QuickSave>\n"
            "<Bool>\n"
                "<default value = \"0\"/>\n"
            "</Bool>\n"
            "<String>\n"
                "<default value = \"nothing\"/>\n"
            "</String>\n"      
            "<Int>\n"
                "<default value = \"0\"/>\n"
            "</Int>\n"
            "<Float>\n"
                "<default value = \"0.0\"/>\n"
            "</Float>\n"
        "</QuickSave>";

    //TODO: Will be subtitute for ModuleFiles functions
    
    // Open/create  quick save file
    std::ofstream file(QUICKSAVE_PATH);

    // Override the file with template context
    file << context;

    // Close file
    file.close();
}