#pragma once
#include <intrin.h>

namespace game
{
	struct Color final
	{
		union
		{
			__m128 c4 = _mm_set_ps1(255);
			struct
			{
				float b;
				float g;
				float r;
				float a;
			};
		};
	};
}
