#pragma once
#include <cmath>
#include <algorithm>

namespace utils
{
	class Mathf final
	{
	public:
		inline static float ConstrainAngle(float f);
		inline static float GetAngle(float a, float b);
		inline static float LerpAngle(float a, float b, float t);

		static constexpr float Lerp(float a, float b, float t = .5f, bool clamped = true);
	};

	constexpr float Mathf::Lerp(const float a, const float b, const float t, const bool clamped)
	{
		auto&& res = a + (b - a) * t;
		return clamped ? std::max(.0f, std::min(res, b)) : res;
	}

	inline float Mathf::ConstrainAngle(float f)
	{
		f = fmod(f + 180, 360);
		if (f < 0)
			f += 360;
		return f - 180;
	}

	inline float Mathf::GetAngle(const float a, const float b)
	{
		float dif = fmod(b - a + 180, 360);
		if (dif < 0)
			dif += 360;
		return dif - 180;
	}

	inline float Mathf::LerpAngle(const float a, const float b, const float t)
	{
		return ConstrainAngle(a + GetAngle(a, b) * t);
	}
}
