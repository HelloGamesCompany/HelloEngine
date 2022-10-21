#pragma once

namespace Htool
{
	template<class T>

	class CommandArray
	{
	public: 
		CommandArray(const int size, const bool isPointer);

		CommandArray(const int size,T value, const bool isPointer);

		~CommandArray();

		void push(T value);

		T undo(bool& successful);

		T redo(bool& successful);

		T& get(const int index);

		int size() { return _size; }

	private:

		int next(int index);

		int previous(int index);

	private:

		T* _arr = nullptr;

		bool _isPointer = false;

		int _capacity = 0, _begin = 0, _end = 0, _current = 0, _size = 0;
	};

	template<class T>
	inline CommandArray<T>::CommandArray(int size, const bool isPointer)
	{
		_isPointer = isPointer;

		_arr = new T[size];

		_capacity = size;

		_begin = _end = _current = 0;
	}

	template<class T>
	inline CommandArray<T>::CommandArray(int size, T value, const bool isPointer)
	{
		_isPointer = isPointer;

		_arr = new T[size];

		_capacity = size;

		_begin = _end = _current = 0;

		for (size_t i = 0; i < _capacity; i++)
		{
			_arr[i] = value;
		}
	}

	template<class T>
	inline CommandArray<T>::~CommandArray()
	{
		//TODO release begin to end
		if(_isPointer)
		{
			for (size_t i = 0; i < _capacity; i++)
			{
				RELEASE(_arr[i]);
			}
		}

		delete[] _arr;
	}

	template<class T>
	inline void CommandArray<T>::push(T value)
	{
		if(!_isPointer)
		{
			_arr[_current] = value;

			_end = _current = next(_current);

			_begin = (_current == _begin) ? next(_begin) : _begin;

			_size = _size >= _capacity ? _capacity : (_size + 1);
		}
		else
		{
			if (_current != _end)
			{
				for (int i = _current; i != _end ; i = next(i))
				{
					RELEASE(_arr[i]);
				}
			}

			_arr[_current] = value;

			_end = _current = next(_current);

			if(_current == _begin)
			{
				RELEASE(_arr[_begin]);

				_begin = next(_begin);
			}

			_size = _size >= _capacity ? _capacity : (_size + 1);
		}
	}

	template<class T>
	inline T CommandArray<T>::undo(bool& successful)
	{
		if (_current == _begin) 
		{
			successful = false;

			return _arr[_current];
		}

		successful = true;

		_size = _size <= 0 ? 0 : (_size - 1);

		_current = previous(_current);

		return _arr[_current];
	}

	template<class T>
	inline T CommandArray<T>::redo(bool& successful)
	{
		if (_current == _end) 
		{
			successful = false;

			return _arr[_current-1];
		}

		successful = true;

		_size = _size >= _capacity ? _capacity : (_size + 1);

		_current = next(_current);

		return _arr[_current-1];
	}

	template<class T>
	inline T& CommandArray<T>::get(const int index)
	{
		if (index >= _capacity) 
		{
			LOG("ERR");
			Console::S_Log("CommandArray::Warning -> index out of range!!!");
		}

		return _arr[index];
	}

	template<class T>
	inline int CommandArray<T>::next(int index)
	{
		if (++index >= _capacity) index = 0;

		return index;
	}

	template<class T>
	inline int CommandArray<T>::previous(int index)
	{
		if (--index < 0) index = _capacity - 1;

		return index;
	}
}