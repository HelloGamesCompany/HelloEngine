#pragma once

class Console
{
public:
	static void Log(const std::string text);

	static const char* GetLog();

	static const char* GetLogCounts();

	static void ClearLog();

private:
	static std::string _buffer;

	static std::string _logCountText;

	static size_t _logCount;
};