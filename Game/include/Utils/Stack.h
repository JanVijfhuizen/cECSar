#pragma once
#include <cstdint>

namespace utils
{
	template <typename T>
	class Stack final
	{
	public:
		constexpr bool IsEmpty() const;

		constexpr Stack(int32_t blockSize = 16);
		~Stack();

		constexpr void Push(T instance);
		constexpr T Pop();

		constexpr void Clear();

	private:
		struct Block final
		{
			T* values = nullptr;
			Block* previous = nullptr;

			Block(int32_t size, Block* next = nullptr);
			~Block();
		};

		Block* _lastBlock = nullptr;
		int32_t _count = 0;
		int32_t _blockSize = 0;
	};

	template <typename T>
	constexpr bool Stack<T>::IsEmpty() const
	{
		return _count == 0;
	}

	template <typename T>
	constexpr Stack<T>::Stack(const int32_t blockSize) : _blockSize(blockSize)
	{
		_lastBlock = new Block(blockSize);
	}

	template <typename T>
	Stack<T>::~Stack()
	{
		delete _lastBlock;
	}

	template <typename T>
	Stack<T>::Block::Block(const int32_t size, Block* next)
	{
		values = new T[size];
		next->previous = this;
	}

	template <typename T>
	Stack<T>::Block::~Block()
	{
		delete previous;
		delete values;
	}

	template <typename T>
	constexpr void Stack<T>::Push(T instance)
	{
		const int32_t index = _count++ % _blockSize;
		if(index == 0)
			_lastBlock = new Block(_blockSize, _lastBlock);
		_lastBlock->values[index] = instance;
	}

	template <typename T>
	constexpr T Stack<T>::Pop()
	{
		const int32_t index = _count-- % _blockSize;
		T ret = _lastBlock[index];

		if(index == 0)
		{
			Block* deletable = _lastBlock;
			_lastBlock = deletable->previous;

			deletable->previous = nullptr;
			delete deletable;
		}

		return ret;
	}

	template <typename T>
	constexpr void Stack<T>::Clear()
	{
		if (IsEmpty())
			return;
		_count = 0;

		delete _lastBlock;
		_lastBlock = new Block(_blockSize);
	}
}
