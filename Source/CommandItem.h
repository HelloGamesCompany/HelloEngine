#pragma once

class Command;

class CommandItem
{
public:
	CommandItem();

	CommandItem(Command* command);

	~CommandItem();

	void Undo();

	void Redo();

private:
	Command* _command = nullptr;
};