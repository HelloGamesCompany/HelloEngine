#pragma once

namespace Htool
{
	template<class T>

	class CommandArray
	{
	public: 
		CommandArray(int size);

		~CommandArray();

		void push(T value);

		T* undo();

		T* redo();

	private:

		int next(int index);

		int previous(int index);

	private:

		T* _arr = nullptr;

		int _size = 0, _begin = 0, _end = 0, _current = 0;
	};
	template<class T>
	inline CommandArray<T>::CommandArray(int size)
	{
		_arr = new T[size];

		_size = size;

		_begin = _end = _current = 0;
	}
	template<class T>
	inline CommandArray<T>::~CommandArray()
	{
		delete _arr;
	}
	template<class T>
	inline void CommandArray<T>::push(T value)
	{
		_arr[_current] = value;

		_end = _current = next(_current);

		_begin = (_current == _begin) ? next(_begin): _begin;
	}
	template<class T>
	inline T* CommandArray<T>::undo()
	{
		if (_current == _begin) return nullptr;

		_current = previous(_current);

		return &_arr[_current];
	}
	template<class T>
	inline T* CommandArray<T>::redo()
	{
		if (_current == _end) return nullptr;

		_current = next(_current);

		return &_arr[_current];
	}
	template<class T>
	inline int CommandArray<T>::next(int index)
	{
		if (++index >= _size) index = 0;

		return index;
	}
	template<class T>
	inline int CommandArray<T>::previous(int index)
	{
		if (--index < 0) index = _size - 1;

		return index;
	}
}