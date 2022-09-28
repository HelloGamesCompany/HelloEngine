#include "Headers.h"
#include "QuickSave.h"
#include "ModuleXML.h"

XMLNode QuickSave::nBool;
XMLNode QuickSave::nString;
XMLNode QuickSave::nFloat;
XMLNode QuickSave::nInt;

void QuickSave::Init()
{
    // Create The XML file while is not exist
    if (!std::filesystem::exists(QUICKSAVE_FILENAME)) CreateQuickSaveDefaultFile();

    LOG("Init QuickSave");

    XMLNode node = Application::Instance()->xml->OpenXML(QUICKSAVE_FILENAME);

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
    SetValue(name, std::to_string(value), nFloat);
}

void QuickSave::SetInt(std::string name, int value)
{
    SetValue(name, std::to_string(value), nInt);
}

bool QuickSave::GetBool(std::string name, bool defaultValue)
{
    std::string value = GetValue(name, std::to_string(defaultValue), nBool);

    return (value == "true");
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

void QuickSave::SetValue(std::string name, std::string value, XMLNode& node)
{
    XMLNode n = node.FindChildBreadth(name);

    // If there wasnt any node with the given name, create one.
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
    if (n != node) ret = n.node.attribute("value").as_string();
    
    return ret;
}

void QuickSave::CreateQuickSaveDefaultFile()
{
    LOG("Create default QuickSave file");

    // Create a new QuickSave xml file
    FILE* f = nullptr;

    fopen_s(&f, QUICKSAVE_FILENAME, "w");

    // Load template QuickSave structure into the new file
    char buffer[] = "<?xml version=\"1.0\"?>\n<QuickSave>\n<Bool>\n<default value = \"0\"/>\n</Bool>\n<String>\n<default value = \"0\"/>\n</String>\n<Float>\n<default value = \"0\"/>\n</Float>\n<Int>\n<default value = \"0\"/>\n</Int>\n</QuickSave>";                            

    fwrite(buffer, sizeof(buffer), 1, f);

    fclose(f);
}