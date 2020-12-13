#pragma once
#include <emmintrin.h>

namespace game
{
	class TransformSystem;
	struct TransformHelper;

	struct Transform final
	{
		friend TransformSystem;
		friend TransformHelper;

#pragma region Local
		union
		{
			__m128 p4 = _mm_set_ps1(0);
			struct
			{
				float x, y, z;
			};
		};

		float rotation = 0;
#pragma endregion

#pragma region Global Readonly
		union
		{
			__m128 p4Global = _mm_set_ps1(0);
			struct
			{
				float xGlobal, yGlobal, zGlobal;
			};
		};

		float rotationGlobal = 0;
#pragma endregion

	private:
		int32_t parent = -1;
		int32_t rDepth = 0;
	};
}
