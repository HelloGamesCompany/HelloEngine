#pragma once

#include <iostream>

namespace Htool 
{
	template<class T>
	class ArrayQueue
	{
	public:
		ArrayQueue(int size);

		ArrayQueue(int size,T initValue);

		~ArrayQueue();

		void PushBack(T var);

		// Get first element direccion memory
		T* Front();

		int GetSize();

	public:
		T* arr = nullptr;

		// halfSize = real size
		int size = 0, capacity = 0;

		int frontIndex = 0, inputIndex = 0;

		bool startDisplace = false;
	};

	template<class T>
	inline ArrayQueue<T>::ArrayQueue(int size)
	{
		this->size = size;

		capacity = size * 2;

		arr = new T[capacity];
	}

	template<class T>
	inline ArrayQueue<T>::ArrayQueue(int size, T initValue)
	{
		this->size = size;

		capacity = size * 2;

		arr = new T[capacity];

		for (int i = 0; i < capacity; i++) arr[i] = initValue;
	}

	template<class T>
	inline ArrayQueue<T>::~ArrayQueue()
	{
		RELEASE(arr);
	}

	template<class T>
	inline void ArrayQueue<T>::PushBack(T var)
	{
		arr[inputIndex] = var;

		if (inputIndex >= size) arr[inputIndex - size] = var;

		// If input is out of size, we need to start displace the currentIndex
		if(!startDisplace && inputIndex >= size) startDisplace = true;
		
		// Displace forntIndex
		if(startDisplace)
		{
			// If frontier is out of range, move to the start position
			if (++frontIndex > size) frontIndex = 0;
		}

		// If inputIndex is out of range, move to the start position
		if (++inputIndex > capacity) inputIndex = size;
	}

	template<class T>
	inline T* ArrayQueue<T>::Front()
	{
		if(arr)	return &arr[frontIndex];

		return nullptr;
	}

	template<class T>
	inline int ArrayQueue<T>::GetSize()
	{
		return size;
	}
}