#pragma once

namespace Htool
{
	template<class T> class CycleArray;
}

using CArrayS = Htool::CycleArray<std::string>;

class Console
{
public:
	static void S_Init();

	static void S_Close();

	static void S_Log(const std::string text);

	static uint S_GetLog(std::string** buffer);

	static std::map<std::string, uint> S_GetCollapseLog();

	static std::string S_GetLastLog();

	static const char* S_GetLogCounts();

	static void S_ClearLog();

	static void S_SaveLog();

private:
	static CArrayS* _buffers;

	static std::map<std::string, uint> _buffersMap;

	static std::string _logCountText;

	static size_t _logCount;
};