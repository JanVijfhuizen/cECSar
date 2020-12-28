#pragma once
#include "Utils/Vector3.h"
#include "EntityInfo.h"

namespace game
{
	struct Kinematic final
	{
		utils::Vector3 offset{};
		float max = 100;
		cecsar::EntityInfo target{};
	};
}
