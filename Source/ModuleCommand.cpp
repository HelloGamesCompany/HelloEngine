#include "Headers.h"
#include "ModuleCommand.h"
#include "CommandArray.hpp"
#include "CommandChangeValue.hpp"
#include "Console.h"

CommandArray* ModuleCommand:: _commands = nullptr;

ModuleCommand::ModuleCommand()
{
	_commands = new CommandArray(MAX_UNDO, true);

	input = Application::Instance()->input;
}

ModuleCommand::~ModuleCommand()
{
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

	command->Redo();

	return successful;
}
