#pragma once
#include "Globals.h"

namespace API
{
	class TO_API API_QuickSave
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="name">: There will ignore all spacebacks</param>
		static void SetBool(const std::string name, const bool value);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="name">: There will ignore all spacebacks</param>
		static void SetString(const std::string name, const std::string value);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="name">: There will ignore all spacebacks</param>
		static void SetFloat(const std::string name, const float value);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="name">: There will ignore all spacebacks</param>
		static void SetInt(const std::string name, const int value);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="name">: There will ignore all spacebacks</param>
		static bool GetBool(const std::string name, const bool defaultValue = false);
		/// <summary>
		/// This function is different from another one, as it requires you to provide a string where the result will be stored
		/// </summary>
		/// <param name="name">: There will ignore all spacebacks</param>
		/// <param name="des">: context that will be returned </param>
		static void GetString(const std::string name, std::string& destination, const std::string defaultValue = "DefaultValue");
		/// <summary>
		/// 
		/// </summary>
		/// <param name="name">: There will ignore all spacebacks</param>
		static float GetFloat(const std::string name, const float defaultValue = 0);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="name">: There will ignore all spacebacks</param>
		static int GetInt(const std::string name, const int defaultValue = 0);

		static void ResetAll();
		static void ResetBool();
		static void ResetFloat();
		static void ResetInt();
		static void ResetString();
	};
}
