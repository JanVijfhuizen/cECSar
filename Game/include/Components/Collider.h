#pragma once
#include <vector>
#include "EntityInfo.h"

namespace game
{
	struct Collider final
	{
		bool isTrigger = false;
		bool isStatic = false;

		float radius = 1;

		std::vector<cecsar::EntityInfo> hits;
	};
}
