#pragma once
#include <cstdint>

namespace utils
{
	template <typename T>
	class Sorter final
	{
	public:
		template <typename SortingMethod>
		constexpr static void Sort(T* arr, int32_t begin, int32_t end, SortingMethod func);
		constexpr static void Swap(T* arr, int32_t a, int32_t b);

	private:
		template <typename SortingMethod>
		constexpr static void QuickSort(T* arr, int32_t low, int32_t high, SortingMethod func);
		template <typename SortingMethod>
		constexpr static int32_t Partition(T* arr, int32_t low, int32_t high, SortingMethod func);
	};

	template <typename T>
	template <typename SortingMethod>
	constexpr void Sorter<T>::Sort(T* arr, 
		const int32_t begin, const int32_t end, const SortingMethod func)
	{
		QuickSort(arr, begin, end, func);
	}

	template <typename T>
	constexpr void Sorter<T>::Swap(T* arr, const int32_t a, const int32_t b)
	{
		const T value = arr[a];
		arr[a] = arr[b];
		arr[b] = value;
	}

	template <typename T>
	template <typename SortingMethod>
	constexpr void Sorter<T>::QuickSort(T* arr, 
		int32_t low, int32_t high, const SortingMethod func)
	{
		if (low > high)
			return;

		const int32_t ptr = Partition(arr, low, high, func);
		QuickSort(arr, low, ptr - 1, func);
		QuickSort(arr, ptr + 1, high, func);
	}

	template <typename T>
	template <typename SortingMethod>
	constexpr int32_t Sorter<T>::Partition(T* arr, 
		const int32_t low, const int32_t high, const SortingMethod func)
	{
		const float pivot = func(arr[high]);
		int32_t i = low - 1;

		for (int32_t j = low; j <= high - 1; ++j)
			if (func(arr[j]) <= pivot)
				Swap(arr, ++i, j);

		++i;
		Swap(arr, i, high);
		return i;;
	}
}
