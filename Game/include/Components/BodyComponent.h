#pragma once
#include "Utils/Vector3.h"
#include <cstdint>

namespace game
{
	enum class BodyMovementType
	{
		none, in, out
	};

	struct BodyComponent final
	{
		float outMoveThreshold = 8;
		float inMoveThreshold = 20;
		float moveSpeed = 200;

		int32_t parent = -1;
		utils::Vector3 offset{};

		BodyMovementType movementType = BodyMovementType::none;
	};
}
