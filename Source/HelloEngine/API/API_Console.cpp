#include "Headers.h"
#include "API_Console.h"

void API::Console::Log(const std::string& text, API::Console::MessageType type)
{
	Engine::Console::S_Log(text, (LogType)type);
}
