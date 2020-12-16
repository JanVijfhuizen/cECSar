#pragma once
#include "Utils/Vector3.h"
#include <cstdint>

namespace game
{
	struct BodyComponent final
	{
		float moveThreshold = 48;
		float stoppingDistance = 5;
		float speedMultiplier = 1.5f;

		int32_t parent = -1;
		utils::Vector3 offset{};

		bool moving = false;
	};
}
