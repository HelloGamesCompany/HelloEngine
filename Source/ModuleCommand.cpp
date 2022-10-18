#include "Headers.h"
#include "ModuleCommand.h"
#include "CommandArray.hpp"

ModuleCommand::ModuleCommand()
{

}

ModuleCommand::~ModuleCommand()
{
	RELEASE(commands);
}