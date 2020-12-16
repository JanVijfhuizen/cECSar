#pragma once
#include <emmintrin.h>

namespace utils
{
	struct Vector3 final
	{
		union
		{
			__m128 v4 = _mm_set_ps1(0);
			struct
			{
				float x, y, z;
			};
		};

		constexpr Vector3();
		constexpr Vector3(__m128 v4);
		constexpr Vector3(float x, float y, float z);

		inline float Magnitude() const;
		inline Vector3 Normalized() const;
	};

	constexpr Vector3::Vector3() = default;

	constexpr Vector3::Vector3(const __m128 v4) : v4(v4)
	{

	}

	constexpr Vector3::Vector3(const float x, const float y = 0, const float z = 0) : 
		x(x), y(y), z(z)
	{
	}

	inline Vector3 Vector3::Normalized() const
	{
		return 
		{ 
			_mm_div_ps(v4, _mm_set_ps1(Magnitude())) 
		};
	}

	inline float Vector3::Magnitude() const
	{
		return sqrtf(x * x + y * y + z * z);
	}
}
