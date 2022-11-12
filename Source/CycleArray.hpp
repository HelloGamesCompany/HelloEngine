#pragma once

namespace Htool 
{
	template<class T>

	/// <summary>
	/// A cycle data struct
	/// </summary>
	class CycleArray
	{
	public:
		CycleArray(int size);

		CycleArray(int size,T initValue);

		~CycleArray();

		/// <summary>
		/// Add value to the last input position, is the position is out of the range, reset input position 
		/// </summary>
		/// <param name="var"></param>
		void push_back(T var);

		// Get first element direccion memory
		T* front();

		void reset();

		int size();

	private:
		T* _arr = nullptr;

		// halfSize = real size
		int _size = 0, _capacity = 0, _currentSize = 0;

		int _frontIndex = 0, _inputIndex = 0;

		bool _isStartDisplace = false;
	};

	template<class T>
	inline CycleArray<T>::CycleArray(int size)
	{
		this->_size = size;

		_capacity = size * 2;

		_arr = new T[_capacity];
	}

	template<class T>
	inline CycleArray<T>::CycleArray(int size, T initValue)
	{
		this->_size = size;

		_capacity = size * 2;

		_arr = new T[_capacity];

		// Init value with init value
		for (int i = 0; i < _capacity; i++) _arr[i] = initValue;
	}

	template<class T>
	inline CycleArray<T>::~CycleArray()
	{
		delete[] _arr;
	}

	template<class T>
	inline void CycleArray<T>::push_back(T var)
	{
		_arr[_inputIndex] = var;

		if (_currentSize < _size)
		{
			++_currentSize;
		}

		// Copy the second zone value to the first zone
		if (_inputIndex >= _size)
		{
			_arr[_inputIndex - _size] = var;
		}

		// If input is out of size, we need to start displace the currentIndex
		if (!_isStartDisplace && _inputIndex >= _size) 
		{
			_isStartDisplace = true;
		}

		// Displace forntIndex
		if(_isStartDisplace)
		{
			// If frontier is out of range, move to the start position
			if (++_frontIndex >= _size)
			{
				_frontIndex = 0;
			}
		}

		// If inputIndex is out of range, move to the start position
		if (++_inputIndex >= _capacity)
		{
			_inputIndex = _size;
		}
	}

	template<class T>
	inline T* CycleArray<T>::front()
	{
		if (_arr)
		{
			return &_arr[_frontIndex];
		}

		return nullptr;
	}

	template<class T>
	inline void CycleArray<T>::reset()
	{
		_currentSize = 0;
	}

	template<class T>
	inline int CycleArray<T>::size()
	{
		return _currentSize;
	}
}