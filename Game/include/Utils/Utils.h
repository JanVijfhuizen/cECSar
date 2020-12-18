#pragma once
#include <cstdint>

namespace utils
{
	class Utils final
	{
	public:
		template <typename T>
		static constexpr void Copy(T* arr, T* out, int32_t from, int32_t to);
	};

	template <typename T>
	constexpr void Utils::Copy(T* arr, T* out, const int32_t from, const int32_t to)
	{
		for (int32_t i = from; i < to; ++i)
			out[i] = arr[i];
	}
}
