#pragma once
#include "Globals.h"

namespace API
{
	class TO_API API_QuickSave
	{
	public:
		static void SetBool(const std::string name, const bool value);
		static void SetString(const std::string name, const std::string value);
		static void SetFloat(const std::string name, const float value);
		static void SetInt(const std::string name, const int value);

		static bool GetBool(const std::string name, const bool defaultValue = false);
		/// <summary>
		/// This function is different from another one, as it requires you to provide a string where the result will be stored
		/// </summary>
		/// <param name="name"></param>
		/// <param name="des"> context that will be returned </param>
		/// <param name="defaultValue"></param>
		static void GetString(const std::string name, std::string& destination, const std::string defaultValue = "DefaultValue");
		static float GetFloat(const std::string name, const float defaultValue = 0);
		static int GetInt(const std::string name, const int defaultValue = 0);
	};
}
