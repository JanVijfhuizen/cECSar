#pragma once
#include <cstdint>

namespace utils
{
	template <typename T>
	class Utils final
	{
	public:
		static constexpr void Copy(T* arr, T* out, int32_t from, int32_t to);

		template <typename FillMethod>
		static constexpr void Fill(T* arr, int32_t from, int32_t to, FillMethod method);
	};

	template <typename T>
	constexpr void Utils<T>::Copy(T* arr, T* out, const int32_t from, const int32_t to)
	{
		for (int32_t i = from; i < to; ++i)
			out[i] = arr[i];
	}

	template <typename T>
	template <typename FillMethod>
	constexpr void Utils<T>::Fill(T* arr, const int32_t from, const int32_t to, const FillMethod method)
	{
		for (int32_t i = from; i < to; ++i)
			arr[i] = method(i);
	}
}
