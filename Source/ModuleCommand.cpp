#include "Headers.h"
#include "ModuleCommand.h"
#include "CommandArray.hpp"
#include "CommandDeleteGameObject.h"
#include "CommandSetParentGameObject.h"
#include "CommandChangeTransform.h"
#include "Console.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"

CommandArray* ModuleCommand:: _commands = nullptr;

ModuleCommand::ModuleCommand()
{
	_commands = new CommandArray(MAX_UNDO, true);

	_input = Application::Instance()->input;
}

ModuleCommand::~ModuleCommand()
{
	RELEASE(_commands);
}

UpdateStatus ModuleCommand::Update()
{
	if(_input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		if(_input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		{
			if (!Undo())
			{
				app->layers->editor->AddPopUpMessage("Cannot undo anymore!!!");
			}
		}
		if (_input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
		{
			if (!Redo()) 
			{
				app->layers->editor->AddPopUpMessage("Cannot redo anymore!!!"); 
			}
		}
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

void ModuleCommand::S_ChangeTransform(float4x4& newGlobalMatrix, float4x4& currentGlobalMatrix, bool ignoreRotation, std::function<void(float4x4&, bool)> function)
{
	std::cout << "Added transform change" << std::endl;
	_commands->push(new CommandChangeTransform(newGlobalMatrix, currentGlobalMatrix, ignoreRotation, function));
}

void ModuleCommand::S_DeleteGameObject(GameObject* gameobject)
{
	if (gameobject != nullptr)
		_commands->push(new CommandDeleteGameObject(gameobject));
}

void ModuleCommand::S_SetParentGameObject(GameObject* gameobject, GameObject* newParent)
{
	_commands->push(new CommandSetParentGameObject(gameobject, newParent));
}

void ModuleCommand::S_CleanCommandQueue()
{
	RELEASE(_commands);
	_commands = new CommandArray(MAX_UNDO, true);
}

bool ModuleCommand::Undo()
{
	bool successful = false;

	Command* command = _commands->undo(successful);

	if(!successful)
	{
		Console::S_Log("Wanrning: Do not undo anymore");

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
