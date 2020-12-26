#pragma once
#include "Utils/Vector3.h"
#include <cstdint>

namespace game
{
	struct LegComponent final
	{
		float softMoveThreshold = 64;
		float hardMoveThreshold = 128;

		float stoppingDistance = 10;
		float speedMultiplier = 2.5f;

		int32_t other = -1;
		int32_t parent = -1;

		utils::Vector3 offset{};

		bool moving = false;
	};
}
