﻿#pragma once
#include <cstdint>
#include <string>

namespace utils
{
	template <typename T>
	class SparseSet;

	template <typename T>
	struct SparseIndexIterator
	{
		friend SparseSet<T>;

	public:
		constexpr int32_t operator[](int32_t denseIndex) const;
		constexpr int32_t GetCount() const;

		int32_t* begin() const;
		int32_t* end() const;

	private:
		int32_t* _dense;
		int32_t _count;

		constexpr SparseIndexIterator(int32_t* dense, int32_t count);
	};

	template <typename T>
	class SparseSet final
	{
	public:
		constexpr T& operator[](int32_t denseIndex) const;

		constexpr SparseIndexIterator<T> GetDenseIterator();
		constexpr bool Contains(int32_t sparseIndex, T& out) const;
		constexpr int32_t GetCount() const;

		constexpr SparseSet(int32_t capacity);
		~SparseSet();

		constexpr int32_t Add(T val = T());
		constexpr T& Insert(int32_t sparseIndex, T val = T());

		constexpr void RemoveAt(int32_t sparseIndex);
		constexpr void Clear();

		constexpr T* begin() const;
		constexpr T* end() const;

		constexpr int32_t ToSparseIndex(int32_t denseIndex) const;
		constexpr int32_t ToDenseIndex(int32_t sparseIndex) const;

	private:
		int32_t* _dense = nullptr;
		int32_t* _sparse = nullptr;
		T* _values = nullptr;

		int32_t _count = 0;
		int32_t _capacity = 0;
	};

	template <typename T>
	constexpr T& SparseSet<T>::operator[](const int32_t denseIndex) const
	{
		return _values[denseIndex];
	}

	template <typename T>
	constexpr SparseIndexIterator<T> SparseSet<T>::GetDenseIterator()
	{
		return {_dense, _count};
	}

	template <typename T>
	constexpr bool SparseSet<T>::Contains(const int32_t sparseIndex, T& out) const
	{
		const int32_t denseIndex = _sparse[sparseIndex];
		if (denseIndex == -1)
			return false;

		out = _values[denseIndex];
		return true;
	}

	template <typename T>
	constexpr int32_t SparseSet<T>::GetCount() const
	{
		return _count;
	}

	template <typename T>
	constexpr SparseSet<T>::SparseSet(const int32_t capacity) :
		_capacity(capacity)
	{
		_dense = new int32_t[capacity];
		_sparse = new int32_t[capacity];
		_values = new T[capacity];

		for (int32_t i = 0; i < capacity; ++i)
			_sparse[i] = -1;
	}

	template <typename T>
	constexpr int32_t SparseSet<T>::Add(T val)
	{
		T t;

		for (int32_t i = 0; i < _capacity; ++i)
		{
			if (Contains(i, t))
				continue;

			const int32_t index = _count++;
			_values[index] = val;

			_dense[index] = i;
			_sparse[i] = index;

			return i;
		}

		return -1;
	}

	template <typename T>
	constexpr T& SparseSet<T>::Insert(const int32_t sparseIndex, T val)
	{
		_sparse[sparseIndex] = _count;
		T& t = _values[_count] = val;
		_dense[_count++] = sparseIndex;
		return t;
	}

	template <typename T>
	constexpr void SparseSet<T>::RemoveAt(const int32_t sparseIndex)
	{
		T t;
		if (!Contains(sparseIndex, t))
			return;

		const int32_t denseIndex = _sparse[sparseIndex];
		const int32_t otherSparseIndex = _dense[denseIndex] = _dense[--_count];

		_sparse[otherSparseIndex] = denseIndex;
		_sparse[sparseIndex] = -1;
		_values[denseIndex] = _values[_count];
	}

	template <typename T>
	constexpr void SparseSet<T>::Clear()
	{
		for (int32_t i = 0; i < _count; ++i)
			_sparse[_dense[i]] = -1;
		_count = 0;
	}

	template <typename T>
	int32_t* SparseIndexIterator<T>::begin() const
	{
		return _dense;
	}

	template <typename T>
	int32_t* SparseIndexIterator<T>::end() const
	{
		return &_dense[_count];
	}

	template <typename T>
	constexpr int32_t SparseIndexIterator<T>::operator[](const int32_t denseIndex) const
	{
		return _dense[denseIndex];
	}

	template <typename T>
	constexpr int32_t SparseIndexIterator<T>::GetCount() const
	{
		return _count;
	}

	template <typename T>
	constexpr SparseIndexIterator<T>::SparseIndexIterator(int32_t* dense, const int32_t count) :
		_dense(dense), _count(count)
	{

	}

	template <typename T>
	SparseSet<T>::~SparseSet()
	{
		delete[] _dense;
		delete[] _sparse;
		delete[] _values;
	}

	template <typename T>
	constexpr T* SparseSet<T>::begin() const
	{
		return &_values[0];
	}

	template <typename T>
	constexpr T* SparseSet<T>::end() const
	{
		return &_values[_count];
	}

	template <typename T>
	constexpr int32_t SparseSet<T>::ToSparseIndex(const int32_t denseIndex) const
	{
		return _dense[denseIndex];
	}

	template <typename T>
	constexpr int32_t SparseSet<T>::ToDenseIndex(const int32_t sparseIndex) const
	{
		return _sparse[sparseIndex];
	}
}
