#include "Headers.h"
#include "Command.h"
#include "CommandItem.h"

CommandItem::CommandItem()
{
}

CommandItem::CommandItem(Command* command)
{
	_command = command;
}

CommandItem::~CommandItem()
{
	RELEASE(_command);
}

void CommandItem::Undo()
{
	if(_command) _command->Undo();
}

void CommandItem::Redo()
{
	if (_command) _command->Redo();
}