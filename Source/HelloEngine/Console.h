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

enum class LogType
{
	INFO,
	WARNING,
	ERR
};

struct ConsoleMessage
{
	ConsoleMessage(std::string m, LogType t) : message(m), type(t) {}
	std::string message;
	LogType type;
};

namespace Engine
{
	class Console
	{
	public:

		static void S_Init();

		static void S_Close();

		//static void S_Log(API::API_Vector2 vec, LogType type = LogType::INFO);

		static void S_Log(API::API_Vector3 vec, LogType type = LogType::INFO);

		static void S_Log(const std::string text, LogType type = LogType::INFO);

		static const std::vector<ConsoleMessage>& S_GetLog();

		static std::map<std::string, uint> S_GetCollapseLog();

		static std::string S_GetLastLog();

		static const char* S_GetLogCounts();

		static void S_ClearLog();

		static void S_SaveLog();

	private:
		static std::vector<ConsoleMessage> _buffers;

		static std::map<std::string, uint> _buffersMap;

		static std::string _logCountText;

		static size_t _logCount;
	};
}





	



