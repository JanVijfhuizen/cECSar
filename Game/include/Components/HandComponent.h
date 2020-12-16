#pragma once

namespace game
{
	struct HandComponent final
	{
		float maxDistance = 16;
		bool follow = false;
		utils::Vector3 target{};
	};
}