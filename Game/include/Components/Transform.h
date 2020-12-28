#pragma once
#include "Utils/Vector3.h"

namespace game
{
	struct Transform final
	{
		utils::Vector3 position;
		float rotation = 0;
		int32_t parent = -1;
	};
}
