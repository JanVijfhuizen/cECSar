#pragma once
#include "Set.h"

namespace jecs
{
	template <typename ... Args>
	class SoASet : public Set<SoASet<Args...>>
	{
	public:
		struct Value final
		{
			const int32_t denseIndex;
			const int32_t sparseIndex;
		};

		class Iterator final
		{
		public:
			Iterator(SoASet<Args...>& set, int32_t index);

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
			SoASet<Args...>& _set;
		};

		static int32_t defaultCapacity;

		explicit SoASet(int32_t capacity = -1);
		~SoASet();

		// Make sort.
		[[nodiscard]] std::tuple<Args...>& GetTupleRaw() const;
		[[nodiscard]] const int32_t* GetDenseRaw() const;
		[[nodiscard]] const int32_t* GetSparseRaw() const;

		template <size_t S>
		[[nodiscard]] constexpr auto At();

		void Insert(int32_t sparseIndex, Args... args);
		void EraseAt(int32_t sparseIndex) override;

		[[nodiscard]] constexpr Iterator begin();
		[[nodiscard]] constexpr Iterator end();

		void Swap(int32_t aDense, int32_t bDense);

		[[nodiscard]] bool Contains(int32_t sparseIndex) const;

	private:
		#define TMPL_INDEX sizeof...(Args) - sizeof...(Tail) - 1

		std::tuple<Args*...> _tuple{};
		int32_t* _dense = nullptr;
		int32_t* _sparse = nullptr;

		int32_t _capacity = -1;
		int32_t _count = 0;

		template <typename Head, typename ...Tail>
		void AllocArrays();
		template <typename Head, typename ...Tail>
		void DeallocArray();

		template <typename Head, typename ...Tail>
		void IterInsert(int32_t denseIndex, Head head, Tail... tail);
		template <typename Head, typename ...Tail>
		void IterErase(int32_t denseIndex);

		template <typename Head, typename ...Tail>
		void IterSwap(int32_t aDense, int32_t bDense);
	};

	template <typename ... Args>
	template <size_t S>
	constexpr auto SoASet<Args...>::At()
	{
		return std::get<S>(_tuple);
	}

	template <typename ... Args>
	constexpr typename SoASet<Args...>::Iterator SoASet<Args...>::begin()
	{
		return Iterator{ *this, 0 };
	}

	template <typename ... Args>
	constexpr typename SoASet<Args...>::Iterator SoASet<Args...>::end()
	{
		return Iterator{ *this, _count };
	}

	template <typename ... Args>
	SoASet<Args...>::Iterator::Iterator(SoASet<Args...>& set, const int32_t index) :
		_index(index), _set(set)
	{

	}

	template <typename ... Args>
	typename SoASet<Args...>::Value SoASet<Args...>::Iterator::operator*() const
	{
		return { _index, _set._dense[_index] };
	}

	template <typename ... Args>
	typename SoASet<Args...>::Value SoASet<Args...>::Iterator::operator->() const
	{
		return { _index, _set._dense[_index] };
	}

	template <typename ... Args>
	const typename SoASet<Args...>::Iterator& SoASet<Args...>::Iterator::operator++()
	{
		++_index;
		return *this;
	}

	template <typename ... Args>
	typename SoASet<Args...>::Iterator SoASet<Args...>::Iterator::operator++(int)
	{
		Iterator temp{ *this };
		++_index;
		return temp;
	}

	template <typename ... Args>
	SoASet<Args...>::SoASet(const int32_t capacity) : 
		_capacity(capacity == -1 ? defaultCapacity : capacity)
	{
		_dense = new int32_t[_capacity];
		_sparse = new int32_t[_capacity];

		for (int32_t i = _capacity - 1; i >= 0; --i)
			_sparse[i] = -1;

		AllocArrays<Args...>();
	}

	template <typename ... Args>
	SoASet<Args...>::~SoASet()
	{
		delete[] _dense;
		delete[] _sparse;

		DeallocArray<Args...>();
	}

	template <typename ... Args>
	std::tuple<Args...>& SoASet<Args...>::GetTupleRaw() const
	{
		return _tuple;
	}

	template <typename ... Args>
	const int32_t* SoASet<Args...>::GetDenseRaw() const
	{
		return _dense;
	}

	template <typename ... Args>
	const int32_t* SoASet<Args...>::GetSparseRaw() const
	{
		return _sparse;
	}

	template <typename ... Args>
	void SoASet<Args...>::Insert(const int32_t sparseIndex, Args... args)
	{
		IterInsert<Args...>(_count, args...);

		_sparse[sparseIndex] = _count;
		_dense[_count++] = sparseIndex;
	}

	template <typename ... Args>
	void SoASet<Args...>::EraseAt(const int32_t sparseIndex)
	{
		if (!Contains(sparseIndex))
			return;

		const int32_t denseIndex = _sparse[sparseIndex];
		Swap(denseIndex, --_count);

		_sparse[sparseIndex] = -1;
		IterErase<Args...>(_count);
	}

	template <typename ... Args>
	void SoASet<Args...>::Swap(const int32_t aDense, const int32_t bDense)
	{
		IterSwap<Args...>(aDense, bDense);

		const int32_t aSparse = _dense[aDense];
		const int32_t bSparse = _dense[aDense] = _dense[bDense];
		_dense[bDense] = aSparse;

		_sparse[aSparse] = bDense;
		_sparse[bSparse] = aDense;
	}

	template <typename ... Args>
	bool SoASet<Args...>::Contains(const int32_t sparseIndex) const
	{
		return _sparse[sparseIndex] != -1;
	}

	template <typename ... Args>
	template <typename Head, typename ...Tail>
	void SoASet<Args...>::AllocArrays()
	{
		std::get<TMPL_INDEX>(_tuple) = new Head[_capacity];

		if constexpr (sizeof...(Tail) > 0)
			AllocArrays<Tail...>();
	}

	template <typename ... Args>
	template <typename Head, typename ...Tail>
	void SoASet<Args...>::DeallocArray()
	{
		delete[] std::get<TMPL_INDEX>(_tuple);

		if constexpr (sizeof...(Tail) > 0)
			DeallocArray<Tail...>();
	}

	template <typename ... Args>
	template <typename Head, typename ... Tail>
	void SoASet<Args...>::IterInsert(const int32_t denseIndex, 
		const Head head, Tail... tail)
	{
		std::get<TMPL_INDEX>(_tuple)[denseIndex] = head;

		if constexpr (sizeof...(Tail) > 0)
			IterInsert<Tail...>(denseIndex, tail...);
	}

	template <typename ... Args>
	template <typename Head, typename ... Tail>
	void SoASet<Args...>::IterErase(const int32_t denseIndex)
	{
		std::get<TMPL_INDEX>(_tuple)[denseIndex] = {};

		if constexpr (sizeof...(Tail) > 0)
			IterErase<Tail...>(denseIndex);
	}

	template <typename ... Args>
	template <typename Head, typename ... Tail>
	void SoASet<Args...>::IterSwap(const int32_t aDense, const int32_t bDense)
	{
		const auto arr = std::get<TMPL_INDEX>(_tuple);

		Head aValue = std::move(arr[aDense]);
		arr[aDense] = std::move(arr[bDense]);
		arr[bDense] = std::move(aValue);

		if constexpr (sizeof...(Tail) > 0)
			IterSwap<Tail...>(aDense, bDense);
	}

	template <typename ... Args>
	int32_t SoASet<Args...>::defaultCapacity = 1e4;
}
