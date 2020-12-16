#pragma once

namespace game
{
	struct HandComponent final
	{
		float maxDistance = 16;
		int32_t target = -1;
		utils::Vector3 offset{};
	};
}