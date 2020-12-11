#pragma once
#include <emmintrin.h>

namespace game
{
	struct Transform final
	{
		union
		{
			__m128 p4 = _mm_set_ps1(0);
			struct 
			{
				float x, y, z, _;
			};
		};

		float rotation = 0;
	};
}
