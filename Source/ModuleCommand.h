#pragma once
#include "Module.h"
#include "CommandChangeValue.hpp"
#include "CommandArray.hpp"

#define MAX_UNDO 30

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
		_commands->push(new CommandChangeValue<T>(variable, beginValue, endValue));
	}

	template<class T>
	static void S_ChangeValue(T* variable, T beginValue, T endValue, std::function<void()> function)
	{
		_commands->push(new CommandChangeValue<T>(variable, beginValue, endValue, function));
	}

private:

	bool Undo();

	bool Redo();

private:
	static CommandArray* _commands;

	ModuleInput* input = nullptr;
};