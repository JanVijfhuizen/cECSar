#pragma once
#include <emmintrin.h>
#include <SDL_stdinc.h>

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
		inline Vector3(const Vector3& other);
		constexpr Vector3(__m128 v4);
		constexpr Vector3(float x, float y, float z);

#pragma region 3d
		inline float Magnitude() const;
		inline Vector3 Normalized() const;

		inline Vector3 Rotate(float rotation) const;
		constexpr Vector3 To2D() const;
#pragma endregion 

#pragma region 2d
		inline float Magnitude2d() const;
		inline Vector3 Normalized2d() const;
		inline float SquaredLength() const;
#pragma endregion 

		inline Vector3& operator =(const Vector3& other);

		inline Vector3 operator +(const Vector3& other) const;
		inline Vector3 operator -(const Vector3& other) const;
		inline Vector3& operator+=(const Vector3& vector3);
		inline Vector3& operator-=(const Vector3& vector3);

		inline Vector3 operator *(float f) const;
	};

	constexpr Vector3::Vector3() = default;

	inline Vector3::Vector3(const Vector3& other)
	{
		v4 = other.v4;
	}

	constexpr Vector3::Vector3(const __m128 v4) : v4(v4)
	{

	}

	constexpr Vector3::Vector3(const float x, const float y = 0, const float z = 0) : 
		x(x), y(y), z(z)
	{
	}

	inline Vector3 Vector3::Rotate(const float rotation) const
	{
		const float rad = rotation * M_PI / 180;
		const float sin = std::sinf(rad);
		const float cos = std::cosf(rad);

		const float xSin = x * sin;
		const float xCos = x * cos;

		const float ySin = y * sin;
		const float yCos = y * cos;

		return 
		{ 
			xCos - ySin, xSin + yCos, z 
		};
	}

	inline float Vector3::Magnitude2d() const
	{
		return sqrtf(x * x + y * y);
	}

	inline Vector3 Vector3::Normalized2d() const
	{
		const float magnitude = Magnitude2d();
		return{ x / magnitude, y / magnitude };
	}

	inline float Vector3::SquaredLength() const
	{
		return x * x + y * y + z * z;
	}

	constexpr Vector3 Vector3::To2D() const
	{
		return{ x, y };
	}

	inline Vector3& Vector3::operator=(const Vector3& other)
	{
		v4 = other.v4;
		return *this;
	}

	inline Vector3 Vector3::operator+(const Vector3& other) const
	{
		return { _mm_add_ps(v4, other.v4) };
	}

	inline Vector3 Vector3::operator-(const Vector3& other) const
	{
		return { _mm_sub_ps(v4, other.v4) };
	}

	inline Vector3& Vector3::operator+=(const Vector3& vector3)
	{
		v4 = _mm_add_ps(v4, vector3.v4);
		return *this;
	}

	inline Vector3& Vector3::operator-=(const Vector3& vector3)
	{
		v4 = _mm_sub_ps(v4, vector3.v4);
		return *this;
	}

	inline Vector3 Vector3::operator*(const float f) const
	{
		return { _mm_mul_ps(v4, _mm_set_ps1(f)) };
	}

	inline Vector3 Vector3::Normalized() const
	{
		return { _mm_div_ps(v4, _mm_set_ps1(Magnitude())) };
	}

	inline float Vector3::Magnitude() const
	{
		return sqrtf(x * x + y * y + z * z);
	}
}
