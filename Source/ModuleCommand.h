#pragma once
#include "Module.h"
#include "CommandChangeValue.hpp"
#include "CommandArray.hpp"

#define MAX_UNDO 30

typedef Htool::CommandArray<Command*> CommandArray;

class ModuleInput;

class ModuleCommand :public Module
{
public:
	ModuleCommand();

	~ModuleCommand();

	UpdateStatus Update() override;

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="variable">: variable you want to change</param>
	/// <param name="beginValue">: value before change</param>
	/// <param name="endValue">: value after change</param>
	template<class T>
	static void S_ChangeValue(T* variable, T beginValue, T endValue)
	{
		_commands->push(new CommandChangeValue<T>(variable, beginValue, endValue));
	}

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="variable">: variable you want to change</param>
	/// <param name="beginValue">: value before change</param>
	/// <param name="endValue">: value after change</param>
	/// <param name="function">: if you want call some function when undo/redo</param>
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