#pragma once

namespace Htool
{
	template<class T>

	// Better use 2 stacks.

	/// <summary>
	/// Command use array
	/// </summary>
	/// <typeparam name="T"></typeparam>
	class CommandArray
	{
	public: 
		/// <summary>
		/// 
		/// </summary>
		/// <param name="size">: max undo action should be size - 1 </param>
		/// <param name="isPointer">: if is pointer, any default value will be nullptr</param>
		CommandArray(const int size, const bool isPointer);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="size">: max undo action should be size - 1 </param>
		/// <param name="value">: default value for init array, just use with no pointer variable </param>
		/// <param name="isPointer">: in the pointer case, any default value will be nullptr, ignore the value</param>
		CommandArray(const int size,T value, const bool isPointer);

		~CommandArray();

		void push(T value);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="successful">: use for check if undo action is realize successful</param>
		/// <returns> return the undo element, do not use if is not seccessful </returns>
		T undo(bool& successful);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="successful">: use for check if redo action is realize successful</param>
		/// <returns> return the undo element, do not use if is not seccessful </returns>
		T redo(bool& successful);

		/// <summary>
		/// Get data with index, use with caution!!!
		/// </summary>
		/// <returns></returns>
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

		if (_isPointer)
		{
			for (int i = 0; i < _capacity; i++)
			{
				_arr[i] = nullptr;
			}
		}
	}

	template<class T>
	inline CommandArray<T>::CommandArray(int size, T value, const bool isPointer)
	{
		_isPointer = isPointer;

		_arr = new T[size];

		_capacity = size;

		_begin = _end = _current = 0;

		if (_isPointer)
		{
			for (int i = 0; i < _capacity; i++)
			{
				_arr[i] = nullptr;
			}
		}
		else
		{
			for (int i = 0; i < _capacity; i++)
			{
				_arr[i] = value;
			}
		}
	}

	template<class T>
	inline CommandArray<T>::~CommandArray()
	{
		if(_isPointer)
		{
			for (int i = 0; i < _capacity; i++)
			{
				delete _arr[i];

				_arr[i] = nullptr;
			}
		}

		delete[] _arr;
	}

	template<class T>
	inline void CommandArray<T>::push(T value)
	{
		if(!_isPointer) // No pointer case
		{
			_arr[_current] = value;

			_end = _current = next(_current);

			_begin = (_current == _begin) ? next(_begin) : _begin;

			_size = _size >= _capacity ? _capacity : (_size + 1);
		}
		else // pointer case
		{
			// free obsolete datas memory
			if (_current != _end) 
			{
				for (int i = _current; i != _end ; i = next(i))
				{
					delete _arr[i];

					_arr[i] = nullptr;
				}
			}

			// add new command
			_arr[_current] = value;

			_end = _current = next(_current);

			// check if the array is full, in the case, we free _begin position data and move this to the next position
			if(_current == _begin)
			{
				delete _arr[_begin];

				_arr[_begin] = nullptr;

				_begin = next(_begin);
			}

			// check & update size
			_size = _size >= _capacity ? _capacity : (_size + 1);
		}
	}

	template<class T>
	inline T CommandArray<T>::undo(bool& successful)
	{
		// check if we can undo more step or not
		if (_current == _begin) 
		{
			successful = false;

			return _arr[_current];
		}

		successful = true;

		_current = previous(_current);

		_size = _size <= 0 ? 0 : (_size - 1);

		return _arr[_current];
	}

	template<class T>
	inline T CommandArray<T>::redo(bool& successful)
	{
		// check if we can redo more step or not
		if (_current == _end) 
		{
			successful = false;

			return _arr[previous(_current)];
		}

		successful = true;

		_size = _size >= _capacity ? _capacity : (_size + 1);

		_current = next(_current);

		return _arr[previous(_current)];
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