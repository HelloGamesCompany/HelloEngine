#include "Headers.h"
#include "Console.h"
#include "ModuleFiles.h"

std::string Console::_buffer = "";
std::string Console::_logCountText = "";
size_t Console::_logCount = 0;

void Console::S_Log(const std::string text)
{
    _buffer += "\nDebug.Log: " + text;

    _logCount++;
}

const char* Console::S_GetLog()
{
    return _buffer.c_str();
}

const char* Console::S_GetLogCounts()
{
    _logCountText = _logCount > 999 ? "999+" : std::to_string(_logCount);

    return _logCountText.c_str();
}

void Console::S_ClearLog()
{
    S_SaveLog();

    _buffer.clear();

    _logCountText.clear();

    _logCount = 0;
}

void Console::S_SaveLog()
{
    ModuleFiles::S_Save("Assets/debug.txt", _buffer.data(), _buffer.size(), false);
}