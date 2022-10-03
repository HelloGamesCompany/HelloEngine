#include "Headers.h"
#include "Console.h"

std::string Console::_buffer = "";
std::string Console::_logCountText = "";
size_t Console::_logCount = 0;

void Console::Log(const std::string text)
{
    _buffer += "\n" + text;

    _logCount++;
}

const char* Console::GetLog()
{
    return _buffer.c_str();
}

const char* Console::GetLogCounts()
{
    _logCountText = _logCount > 999 ? "999+" : std::to_string(_logCount);

    return _logCountText.c_str();
}