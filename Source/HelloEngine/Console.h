#pragma once

#include "Globals.h"
#include <map>

#define LOG_PATH "Resources/Editor/debug.txt"
#define MAX_CONSOLE_LOGS 3000

namespace Htool
{
	template<class T> class CycleArray;
}

using CArrayS = Htool::CycleArray<std::string>;

namespace Engine
{
	class Console
	{
	public:

		static void S_Init();

		static void S_Close();

		static void S_Log(const std::string text);

		static const std::vector<std::string>& S_GetLog();

		static std::map<std::string, uint> S_GetCollapseLog();

		static std::string S_GetLastLog();

		static const char* S_GetLogCounts();

		static void S_ClearLog();

		static void S_SaveLog();

	private:
		static std::vector<std::string> _buffers;

		static std::map<std::string, uint> _buffersMap;

		static std::string _logCountText;

		static size_t _logCount;
	};
}





	



