#include "Headers.h"
#include "ModuleFiles.h"
#include "CycleArray.hpp"

CArrayS* Console::_buffers = nullptr;
std::map<std::string, uint> Console::_buffersMap;
std::string Console::_logCountText = "";
size_t Console::_logCount = 0;

void Console::S_Init()
{
    static bool isInit = false;

    if (isInit) return;

    _buffers = new CArrayS(MAX_CONSOLE_LOGS);
}

void Console::S_Close()
{
    static bool isClosed = false;

    if (isClosed) return;

    RELEASE(_buffers);
}

extern "C"
{
    __declspec(dllexport) void Console::S_Log(const std::string text)
    {
        _buffers->push_back("\nDebug.Log: " + text);

        auto it = _buffersMap.find(text);

        if (it == _buffersMap.end()) _buffersMap.insert(std::make_pair(text, 0));

        _buffersMap[text]++;

        _logCount++;
    }
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
    std::string buffer="DEBUG INFO:\n";

    // Read all context in the _buffers and put into buffer
    auto* b = _buffers->front();

    for (uint i = 0; i < _buffers->size(); i++, b++) buffer += *b;

    // Convert string buffer to char* buffer
    uint n = buffer.size() + 1;

    char* arr = new char[n];

    strcpy_s(arr, n, buffer.c_str());

    // Save buffer info and release unnecessary memory
    ModuleFiles::S_Save(LOG_PATH, arr, n, false);

    RELEASE_ARRAY(arr);
}