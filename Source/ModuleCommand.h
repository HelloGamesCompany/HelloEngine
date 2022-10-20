#pragma once
#include "Module.h"
#include "CommandChangeValue.hpp"
#include "CommandArray.hpp"

#define MAX_UNDO 20

using CommandArray = Htool::CommandArray<Command*>;

class ModuleInput;

class ModuleCommand :public Module
{
public:
	ModuleCommand();

	~ModuleCommand();

	UpdateStatus Update() override;

	template<class T>
	static void S_ChangeValue(T* variable, T beginValue, T endValue)
	{
		ClearObsoleteCommands();
		_commands->push(new CommandChangeValue<T>(variable, beginValue, endValue));
	}

private:

	bool Undo();

	bool Redo();

	static void ClearObsoleteCommands();

private:
	static CommandArray* _commands;

	ModuleInput* input = nullptr;

	static std::vector<Command*> _commandPendingToDelete;
};