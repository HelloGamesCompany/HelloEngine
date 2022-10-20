#include "Headers.h"
#include "ModuleCommand.h"
#include "CommandArray.hpp"
#include "CommandChangeValue.hpp"
#include "Console.h"

CommandArray* ModuleCommand:: _commands = nullptr;

std::vector<Command*> ModuleCommand::_commandPendingToDelete;

ModuleCommand::ModuleCommand()
{
	_commands = new CommandArray(MAX_UNDO);

	_commandPendingToDelete.reserve(MAX_UNDO);

	input = Application::Instance()->input;
}

ModuleCommand::~ModuleCommand()
{
	for (size_t i = 0; i < _commands->size(); i++)
	{
		RELEASE(_commands->get(i));
	}

	ClearObsoleteCommands();
	
	RELEASE(_commands);
}

UpdateStatus ModuleCommand::Update()
{
	if(input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		if(input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		{
			Undo();
		}
		if (input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
		{
			Redo();
		}
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

bool ModuleCommand::Undo()
{
	bool successful = false;

	Command* command = _commands->undo(successful);

	if(!successful)
	{
		Console::S_Log("Wanrning: Do not undo more");

		return successful;
	}

	_commandPendingToDelete.push_back(command);

	command->Undo();

	return successful;
}

bool ModuleCommand::Redo()
{
	bool successful = false;

	Command* command = _commands->redo(successful);

	if (!successful)
	{
		Console::S_Log("Wanrning: Do not redo more");

		return successful;
	}

	_commandPendingToDelete.erase(_commandPendingToDelete.begin() + _commandPendingToDelete.size() - 1);

	command->Redo();

	return successful;
}

void ModuleCommand::ClearObsoleteCommands()
{
	for (size_t i = 0; i < _commandPendingToDelete.size(); i++)
	{
		RELEASE(_commandPendingToDelete[i]);
	}
}