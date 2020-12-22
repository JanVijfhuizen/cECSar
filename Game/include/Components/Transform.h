#pragma once
#include "Utils/Vector3.h"

namespace game
{
	class TransformSystem;
	struct TransformHelper;

	struct Transform final
	{
		friend TransformSystem;
		friend TransformHelper;

#pragma region Local
		utils::Vector3 posLocal;
		float rotLocal = 0;
#pragma endregion

#pragma region Global Readonly
		utils::Vector3 posGlobal;
		float rotGlobal = 0;
#pragma endregion

	private:
		int32_t parent = -1;
		int32_t rDepth = 0;
	};
}
