#pragma once

class Console
{
public:
	static void S_Log(const std::string text);

	static const char* S_GetLog();

	static const char* S_GetLogCounts();

	static void S_ClearLog();

	static void S_SaveLog();

private:
	static std::string _buffer;

	static std::string _logCountText;

	static size_t _logCount;
};