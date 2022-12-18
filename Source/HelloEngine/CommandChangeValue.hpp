#pragma once
#include "Command.h"

template <class T>
class CommandChangeValue : public Command
{
public:
	CommandChangeValue();

	CommandChangeValue(T* variable, T beginValue, T endValue);

	CommandChangeValue(T* variable, T beginValue, T endValue, std::function<void()> func);

	~CommandChangeValue();

	void Undo() override;

	void Redo() override;

private:

	T* _variable = nullptr;

	T _beginValue;

	T _endValue;

	// for update tranform
	std::function<void()> _func;

	bool _haveFunc = false;
};

template<class T>
inline CommandChangeValue<T>::CommandChangeValue()
{
}

template<class T>
inline CommandChangeValue<T>::CommandChangeValue(T* variable, T beginValue, T endValue)
{
	_variable = variable;

	_beginValue = beginValue;

	_endValue = endValue;

	*_variable = _endValue;
}

template<class T>
inline CommandChangeValue<T>::CommandChangeValue(T* variable, T beginValue, T endValue, std::function<void()> func)
{
	_variable = variable;

	_beginValue = beginValue;

	_endValue = endValue;

	*_variable = _endValue;

	_func = func;

	_haveFunc = true;
}

template<class T>
inline CommandChangeValue<T>::~CommandChangeValue()
{
}

template<class T>
inline void CommandChangeValue<T>::Undo()
{
	*_variable = _beginValue;

	if (_haveFunc) _func();
}

template<class T>
inline void CommandChangeValue<T>::Redo()
{
	*_variable = _endValue;

	if (_haveFunc) _func();
}