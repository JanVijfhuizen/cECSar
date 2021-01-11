#pragma once
#include <cstdint>

namespace game
{
	struct BodyPart final
	{
		int32_t root = -1;
		int32_t mirror = -1;

		float moveDuration = 1;
		bool moving = false;

		float moveThreshold = 8;
		float teleportThreshold = 16;
	};
}
