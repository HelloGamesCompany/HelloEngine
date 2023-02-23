#include "Headers.h"
#include "API_Console.h"

void API::Console::Log(const std::string& text)
{
	Engine::Console::S_Log(text);
}
