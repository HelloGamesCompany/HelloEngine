#include "Headers.h"
#include "ModuleFiles.h"
#include "CycleArray.hpp"

CArrayS* Console::_buffers = nullptr;
std::map<std::string, uint> Console::_buffersMap;
std::string Console::_logCountText = "";
size_t Console::_logCount = 0;

void Console::S_Init()
{
    static bool isInited = false;

    if (isInited) return;

    _buffers = new CArrayS(3000);
}

void Console::S_Close()
{
    static bool isClosed = false;

    if (isClosed) return;

    RELEASE(_buffers);
}

void Console::S_Log(const std::string text)
{
    _buffers->push_back("\nDebug.Log: " + text);

    auto it = _buffersMap.find(text);

    if (it == _buffersMap.end()) _buffersMap.insert(std::make_pair(text, 0));

    _buffersMap[text]++;

    _logCount++;
}

uint Console::S_GetLog(std::string** buffer)
{
    *buffer = _buffers->front();

    return _buffers->size();
}

std::map<std::string, uint> Console::S_GetCollapseLog()
{
    return _buffersMap;
}

std::string Console::S_GetLastLog()
{
    return *(_buffers->front() + _buffers->size());
}

const char* Console::S_GetLogCounts()
{
    _logCountText = _logCount > 999 ? "999+" : std::to_string(_logCount);

    return _logCountText.c_str();
}

void Console::S_ClearLog()
{
    _buffers->reset();

    _buffersMap.clear();

    _logCountText.clear();

    _logCount = 0;
}

void Console::S_SaveLog()
{
    //ModuleFiles::S_Save("Assets/debug.txt", _buffer.data(), _buffer.size(), false);
}