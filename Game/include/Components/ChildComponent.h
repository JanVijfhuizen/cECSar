#pragma once
#include "Transform.h"

namespace game
{
	struct ChildComponent final
	{
		bool updateManually = false;
		int32_t parent = -1;
		Transform localTransform{};
	};
}
