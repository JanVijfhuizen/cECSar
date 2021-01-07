#pragma once
#include "Vector3.h"

namespace utils
{
	/*
	Used for intersection checks in the QuadTree.
	*/
	struct Quad final
	{
		Vector3 pos{};
		float width = 0, height = 0;

		constexpr Quad();
		inline Quad(const Vector3& pos, float width, float height);
	};

	constexpr Quad::Quad() = default;

	inline Quad::Quad(const Vector3& pos, const float width, const float height) :
		pos(pos), width(width), height(height)
	{

	}
}
