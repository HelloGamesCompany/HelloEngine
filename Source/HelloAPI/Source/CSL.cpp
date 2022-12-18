#include "CSL.h"
#include "HelloEngine/Console.h"

void API::CSL::Log(const std::string text)
{
	Engine::Console::S_Log(text);
}

