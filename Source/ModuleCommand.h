#pragma once
#include "Module.h"
#include "Command.h"

namespace Htool
{
	template<class T> class CommandArray;
}

using CommandC = Htool::CommandArray<Command>;

class ModuleCommand :public Module
{
public:

	ModuleCommand();

	~ModuleCommand();

private:

	CommandC* commands = nullptr;
};