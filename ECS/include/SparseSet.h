#pragma once
#include <cstdint>
#include <string>

namespace utils
{
	/*
	This is a sort of combination between a normal vector and a map.
	You can iterate over the contents, but you can also very quickly get instances by index.
	*/
	template <typename T>
	class SparseSet final
	{
	public:
		typedef float (*Sorter)(const T& instance);
		constexpr T& operator[](int32_t denseIndex) const;

		/*
		Get the dense indexes. Each value points to the sparse array and can be used
		with the method Get() to get the instance at a target index.
		*/
		constexpr int32_t* GetDenseRaw() const;

		/*
		Check if the sparse set contains an object at target index.
		*/
		constexpr bool Contains(int32_t sparseIndex) const;
		/*
		Gets the count of the dense array.
		*/
		constexpr int32_t GetCount() const;
		/*
		Gets the capacity, or the count of the sparse array.
		*/
		constexpr int32_t GetCapacity() const;

		constexpr SparseSet(int32_t capacity);
		~SparseSet();

		/*
		Add an instance to the sparse set.
		Will fill in the first empty slot.
		*/
		constexpr int32_t Add(T val = T());
		/*
		Insert an instance at target index.
		*/
		constexpr T& Insert(int32_t sparseIndex, T val = T());
		/*
		Get an entity at target index.
		*/
		constexpr T& Get(int32_t sparseIndex);

		/*
		Remove an entity at target index.
		*/
		constexpr void RemoveAt(int32_t sparseIndex);
		/*
		Clear the set.
		*/
		constexpr void Clear();

		constexpr T* begin() const;
		constexpr T* end() const;

		/*
		Swap two values at target indexes.
		*/
		constexpr void Swap(int32_t a, int32_t b);

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
	constexpr int32_t* SparseSet<T>::GetDenseRaw() const
	{
		return _dense;
	}

	template <typename T>
	constexpr bool SparseSet<T>::Contains(const int32_t sparseIndex) const
	{
		const int32_t denseIndex = _sparse[sparseIndex];
		return denseIndex != -1;
	}

	template <typename T>
	constexpr int32_t SparseSet<T>::GetCount() const
	{
		return _count;
	}

	template <typename T>
	constexpr int32_t SparseSet<T>::GetCapacity() const
	{
		return _capacity;
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
		// Check if the index is filled.
		// TODO: optimize by using quick injection.
		for (int32_t i = 0; i < _capacity; ++i)
		{
			if (Contains(i))
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
	constexpr T& SparseSet<T>::Get(int32_t sparseIndex)
	{
		return _values[_sparse[sparseIndex]];
	}

	template <typename T>
	constexpr void SparseSet<T>::RemoveAt(const int32_t sparseIndex)
	{
		if (!Contains(sparseIndex))
			return;

		const int32_t denseIndex = _sparse[sparseIndex];
		Swap(denseIndex, --_count);

		_sparse[sparseIndex] = -1;
	}

	template <typename T>
	constexpr void SparseSet<T>::Clear()
	{
		for (int32_t i = 0; i < _count; ++i)
			_sparse[_dense[i]] = -1;
		_count = 0;
	}

	template <typename T>
	SparseSet<T>::~SparseSet()
	{
		delete[] _dense;
		delete[] _sparse;
		delete[] _values;
	}

	template <typename T>
	constexpr void SparseSet<T>::Swap(const int32_t a, const int32_t b)
	{
		const int32_t aSparse = _dense[a];
		const int32_t bSparse = _dense[a] = _dense[b];
		_dense[b] = aSparse;

		const T aValue = _values[a];
		_values[a] = _values[b];
		_values[b] = aValue;

		_sparse[aSparse] = b;
		_sparse[bSparse] = a;
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
}
