#pragma once
#include <Utils/Vector3.h>

namespace game
{
	struct RigidBody final
	{
		utils::Vector3 force{};
		float friction = 0.8f;

		float weight = 1;
	};
}
