#pragma once
#include <emmintrin.h>
#include <SDL_stdinc.h>
#include "Mathf.h"
#include <algorithm>

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
		constexpr Vector3(float x, float y = 0, float z = 0);

#pragma region 3d
		inline float Magnitude() const;
		inline Vector3 Normalized() const;

		inline Vector3 Rotate(float rotation) const;
		constexpr Vector3 To2D() const;

		inline Vector3 Reduce(const Vector3& other) const;
#pragma endregion 

#pragma region 2d
		inline float Magnitude2d() const;
		inline Vector3 Normalized2d() const;
		inline float SquaredLength() const;

		inline Vector3 Lerp2d(const Vector3& other, const float& f) const;
		inline Vector3 MoveTowards2d(const Vector3& other, const float& f) const;
		inline Vector3 MoveTowards2dClamped(const Vector3& other, const float& f) const;
#pragma endregion 

		inline Vector3& operator =(const Vector3& other);

		inline Vector3 operator +(const Vector3& other) const;
		inline Vector3& operator+=(const Vector3& vector3);

		inline Vector3 operator -(const Vector3& other) const;
		inline Vector3& operator-=(const Vector3& vector3);

		inline Vector3 operator *(const Vector3& other) const;
		inline Vector3& operator*=(const Vector3& other);

		inline Vector3 operator /(const Vector3& other) const;
		inline Vector3& operator/=(const Vector3& other);

		inline Vector3 operator *(const float& f) const;
		inline Vector3& operator*=(const float& f);

		inline Vector3 operator /(const float& f) const;
		inline Vector3& operator/=(const float& f);

#pragma region Predefined Constructors
		static constexpr Vector3 One();
		static constexpr Vector3 One2d();
#pragma endregion 

#pragma region Statics
		inline static float RotateTowards2d(const float& f, const Vector3& to, const float& delta);
#pragma endregion 
	};

	constexpr Vector3::Vector3() = default;

	inline Vector3::Vector3(const Vector3& other)
	{
		v4 = other.v4;
	}

	constexpr Vector3::Vector3(const __m128 v4) : v4(v4)
	{

	}

	constexpr Vector3::Vector3(const float x, const float y, const float z) : 
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

	inline Vector3 Vector3::Lerp2d(const Vector3& other, const float& f) const
	{
		auto&& lerpable = _mm_sub_ps(other.v4, v4);
		return { _mm_add_ps(v4, _mm_mul_ps(lerpable, _mm_set_ps1(f))) };
	}

	inline Vector3 Vector3::MoveTowards2d(const Vector3& other, const float& f) const
	{
		const auto dir = (other - *this).Normalized2d();
		return *this + dir * f;
	}

	inline Vector3 Vector3::MoveTowards2dClamped(const Vector3& other, const float& f) const
	{
		const float distance = (*this - other).Magnitude2d();
		return MoveTowards2d(other, std::min(f, distance));
	}

	constexpr Vector3 Vector3::To2D() const
	{
		return{ x, y };
	}

	constexpr Vector3 Vector3::One()
	{
		return Vector3(1, 1, 1);
	}

	constexpr Vector3 Vector3::One2d()
	{
		return Vector3(1, 1);
	}

	inline Vector3 Vector3::Reduce(const Vector3& other) const
	{
#define REDUCE(a) a - other.a * (a > 0 ? 1 : -1)
#define CLAMP(a) abs(a) < other.a ? 0 : a
#define AXIS(a) CLAMP(REDUCE(a))

		return Vector3(AXIS(x), AXIS(y), AXIS(z));
	}

	inline Vector3& Vector3::operator=(const Vector3& other)
	{
		v4 = other.v4;
		return *this;
	}

	inline Vector3 Vector3::operator+(const Vector3& other) const
	{
		return _mm_add_ps(v4, other.v4);
	}

	inline Vector3 Vector3::operator-(const Vector3& other) const
	{
		return _mm_sub_ps(v4, other.v4);
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

	inline Vector3 Vector3::operator*(const Vector3& other) const
	{
		return _mm_mul_ps(v4, other.v4);
	}

	inline Vector3& Vector3::operator*=(const Vector3& other)
	{
		v4 = _mm_mul_ps(v4, other.v4);
		return *this;
	}

	inline Vector3 Vector3::operator/(const Vector3& other) const
	{
		return _mm_div_ps(v4, other.v4);
	}

	inline Vector3& Vector3::operator/=(const Vector3& other)
	{
		v4 = _mm_div_ps(v4, other.v4);
		return *this;
	}

	inline Vector3 Vector3::operator*(const float& f) const
	{
		return _mm_mul_ps(v4, _mm_set_ps1(f));
	}

	inline Vector3& Vector3::operator*=(const float& f)
	{
		v4 = _mm_mul_ps(v4, _mm_set_ps1(f));
		return *this;
	}

	inline Vector3 Vector3::operator/(const float& f) const
	{
		return _mm_div_ps(v4, _mm_set_ps1(f));
	}

	inline Vector3& Vector3::operator/=(const float& f)
	{
		v4 = _mm_div_ps(v4, _mm_set_ps1(f));
		return *this;
	}

	inline float Vector3::RotateTowards2d(const float& f, const Vector3& to, const float& delta)
	{
		float targetDegrees = atan2f(-to.x, to.y) * 180 / M_PI;
		targetDegrees = Mathf::ConstrainAngle(targetDegrees);

		const float diff = Mathf::GetAngle(f, targetDegrees);
		const float rotDelta = delta * (diff > 0 ? 1 : -1);

		// Clamp rotation to the size of the angle.
		return f + (abs(diff) < abs(rotDelta) ? diff : rotDelta);
	}

	inline Vector3 Vector3::Normalized() const
	{
		return _mm_div_ps(v4, _mm_set_ps1(Magnitude()));
	}

	inline float Vector3::Magnitude() const
	{
		return sqrtf(x * x + y * y + z * z);
	}
}
