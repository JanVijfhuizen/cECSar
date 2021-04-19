#pragma once
#include <cstdint>
#include "Set.h"

namespace jecs
{
	template <typename T>
	class SparseSet final : public Set<SparseSet<T>>
	{
	public:
		static int32_t defaultCapacity;

		struct Value final
		{
			T& value;
			const int32_t index;
		};

		class Iterator final
		{
		public:
			Iterator(SparseSet<T>& set, int32_t index);

			Value operator*() const;
			Value operator->() const;

			const Iterator& operator++();
			Iterator operator++(int);

			friend auto operator==(const Iterator& a, const Iterator& b) -> bool
			{
				return a._index == b._index;
			};

			friend bool operator!= (const Iterator& a, const Iterator& b)
			{
				return !(a == b);
			};

		private:
			int32_t _index = 0;
			SparseSet<T>& _set;
		};

		typedef int32_t (*Sorter)(const T& a, const T& b, int32_t aIndex, int32_t bIndex);

		explicit constexpr SparseSet(int32_t capacity = -1);
		~SparseSet();

		[[nodiscard]] constexpr T& operator[](int32_t sparseIndex) const;

		[[nodiscard]] constexpr const T* GetValuesRaw() const;
		[[nodiscard]] constexpr const int32_t* GetDenseRaw() const;
		[[nodiscard]] constexpr const int32_t* GetSparseRaw() const;

		[[nodiscard]] constexpr bool Contains(int32_t sparseIndex) const;

		[[nodiscard]] constexpr int32_t GetCount() const;
		[[nodiscard]] constexpr int32_t GetCapacity() const;

		constexpr T& Insert(int32_t sparseIndex, const T& val);
		constexpr T& Insert(int32_t sparseIndex, T&& val = T());

		constexpr void EraseAt(int32_t sparseIndex) override;

		constexpr void Clear();

		[[nodiscard]] constexpr Iterator begin();
		[[nodiscard]] constexpr Iterator end();

		constexpr void Swap(int32_t aDense, int32_t bDense);
		constexpr void Sort(const Sorter& sorter, int32_t start = 0, int32_t stop = -1);

	private:
		int32_t* _dense = nullptr;
		int32_t* _sparse = nullptr;
		T* _values = nullptr;

		int32_t _count = 0;
		int32_t _capacity = 0;
	};

	template <typename T>
	constexpr T& SparseSet<T>::operator[](const int32_t sparseIndex) const
	{
		return _values[_sparse[sparseIndex]];
	}

	template <typename T>
	constexpr const T* SparseSet<T>::GetValuesRaw() const
	{
		return _values;
	}

	template <typename T>
	constexpr const int32_t* SparseSet<T>::GetDenseRaw() const
	{
		return _dense;
	}

	template <typename T>
	constexpr const int32_t* SparseSet<T>::GetSparseRaw() const
	{
		return _sparse;
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
		_capacity(capacity == -1 ? defaultCapacity : capacity)
	{
		_dense = new int32_t[_capacity];
		_sparse = new int32_t[_capacity];
		_values = new T[_capacity];

		for (int32_t i = 0; i < _capacity; ++i)
			_sparse[i] = -1;
	}

	template <typename T>
	constexpr T& SparseSet<T>::Insert(int32_t sparseIndex, const T& val)
	{
		_sparse[sparseIndex] = _count;
		T& t = _values[_count] = val;
		_dense[_count++] = sparseIndex;
		return t;
	}

	template <typename T>
	constexpr T& SparseSet<T>::Insert(const int32_t sparseIndex, T&& val)
	{
		_sparse[sparseIndex] = _count;
		T& t = _values[_count] = std::move(val);
		_dense[_count++] = sparseIndex;
		return t;
	}

	template <typename T>
	constexpr void SparseSet<T>::EraseAt(const int32_t sparseIndex)
	{
		if (!Contains(sparseIndex))
			return;

		const int32_t denseIndex = _sparse[sparseIndex];
		Swap(denseIndex, --_count);

		_sparse[sparseIndex] = -1;
		_values[_count] = T();
	}

	template <typename T>
	constexpr void SparseSet<T>::Clear()
	{
		for (int32_t i = 0; i < _count; ++i) 
		{
			_sparse[_dense[i]] = -1;
			_values[i] = T();
		}
		_count = 0;
	}

	template <typename T>
	SparseSet<T>::Iterator::Iterator(SparseSet<T>& set, const int32_t index):
		_index(index), _set(set)
	{
	}

	template <typename T>
	typename SparseSet<T>::Value SparseSet<T>::Iterator::operator*() const
	{
		return {_set._values[_index], _set._dense[_index]};
	}

	template <typename T>
	typename SparseSet<T>::Value SparseSet<T>::Iterator::operator->() const
	{
		return {_set._values[_index], _set._dense[_index]};
	}

	template <typename T>
	const typename SparseSet<T>::Iterator& SparseSet<T>::Iterator::operator++()
	{
		++_index;
		return *this;
	}

	template <typename T>
	typename SparseSet<T>::Iterator SparseSet<T>::Iterator::operator++(int)
	{
		Iterator temp{*this};
		++_index;
		return temp;
	}

	template <typename T>
	SparseSet<T>::~SparseSet()
	{
		delete[] _dense;
		delete[] _sparse;
		delete[] _values;
	}

	template <typename T>
	constexpr void SparseSet<T>::Swap(const int32_t aDense, const int32_t bDense)
	{
		const int32_t aSparse = _dense[aDense];
		const int32_t bSparse = _dense[aDense] = _dense[bDense];
		_dense[bDense] = aSparse;

		T aValue = std::move(_values[aDense]);
		_values[aDense] = std::move(_values[bDense]);
		_values[bDense] = std::move(aValue);

		_sparse[aSparse] = bDense;
		_sparse[bSparse] = aDense;
	}

	template <typename T>
	constexpr void SparseSet<T>::Sort(const Sorter& sorter,
		const int32_t start, int32_t stop)
	{
		if (stop == -1)
			stop = _count;

		for (int32_t i = stop - 1; i >= start; --i)
			for (int32_t j = i + 1; j < stop; ++j)
			{
				const int32_t index = _dense[j - 1];
				const int32_t otherIndex = _dense[j];

				auto& instance = _values[j - 1];
				auto& other = _values[j];

				if (sorter(instance, other, index, otherIndex) <= 0)
					break;
				Swap(j - 1, j);
			}
	}

	template <typename T>
	constexpr typename SparseSet<T>::Iterator SparseSet<T>::begin()
	{
		return Iterator{ *this, 0 };
	}

	template <typename T>
	constexpr typename SparseSet<T>::Iterator SparseSet<T>::end()
	{
		return Iterator{ *this, _count };
	}

	template <typename T>
	int32_t SparseSet<T>::defaultCapacity = 1e4;
}
