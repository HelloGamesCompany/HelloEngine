#include "Headers.h"
#include "API_QuickSave.h"
#include "QuickSave.h"

void API::API_QuickSave::SetBool(const std::string name, bool value)
{
	QuickSave::SetBool(name.c_str(), value);
}

void API::API_QuickSave::SetString(const std::string name, const std::string value)
{
	QuickSave::SetString(name.c_str(), value);
}

void API::API_QuickSave::SetFloat(const std::string name, const float value)
{
	QuickSave::SetFloat(name.c_str(), value);
}

void API::API_QuickSave::SetInt(const std::string name, const int value)
{
	QuickSave::SetInt(name.c_str(), value);
}

bool API::API_QuickSave::GetBool(const std::string name, const bool defaultValue)
{
	return 	QuickSave::GetBool(name.c_str(), defaultValue);
}

void API::API_QuickSave::GetString(const std::string name, std::string& des, const std::string defaultValue)
{
	const char* temp = QuickSave::GetString(name.c_str(), defaultValue.c_str());

	des = (temp);

	RELEASE_ARRAY(temp);
}

float API::API_QuickSave::GetFloat(const std::string name, const float defaultValue)
{
	return 	QuickSave::GetFloat(name.c_str(), defaultValue);
}

int API::API_QuickSave::GetInt(const std::string name, const int defaultValue)
{
	return 	QuickSave::GetInt(name.c_str(), defaultValue);
}

void API::API_QuickSave::ResetAll()
{
	QuickSave::ResetAllNodes();
}

void API::API_QuickSave::ResetBool()
{
	QuickSave::ResetBoolNode();
}

void API::API_QuickSave::ResetFloat()
{
	QuickSave::ResetFloatNode();
}

void API::API_QuickSave::ResetInt()
{
	QuickSave::ResetIntNode();
}

void API::API_QuickSave::ResetString()
{
	QuickSave::ResetStringNode();
}
