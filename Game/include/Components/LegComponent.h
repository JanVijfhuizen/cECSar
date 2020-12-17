#pragma once
#include "Utils/Vector3.h"
#include <cstdint>

namespace game
{
	struct LegComponent final
	{
		float moveThreshold = 32;
		float teleportThreshold = 128;

		float stoppingDistance = 5;
		float speedMultiplier = 2.5f;

		int32_t other = -1;
		int32_t parent = -1;

		utils::Vector3 offset{};

		bool moving = false;
	};
}
