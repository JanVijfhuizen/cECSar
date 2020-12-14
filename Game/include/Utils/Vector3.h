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
	};
}
