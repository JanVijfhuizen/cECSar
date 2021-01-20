#pragma once
#include "EntityInfo.h"
#include "Utils/Vector3.h"

namespace game
{
	struct Joint final
	{
		cecsar::EntityInfo other{};
		utils::Vector3 offset{};

		float maxDistance = 16;
		float teleportDistance = 128;
	};
}
