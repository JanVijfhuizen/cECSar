#pragma once
#include "Utils/Vector3.h"
#include <cstdint>

namespace game
{
	struct BodyComponent final
	{
		float moveThreshold = 20;
		float stoppingDistance = 10;
		float moveSpeed = 200;

		int32_t parent = -1;
		utils::Vector3 offset{};

		bool moving = false;
	};
}
