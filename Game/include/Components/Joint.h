#pragma once
#include "EntityInfo.h"
#include "Utils/Vector3.h"

namespace game
{
	struct Joint final
	{
		cecsar::EntityInfo other{};
		utils::Vector3 offset{};

		float maxDistance = 32;
		float teleportDistance = 32;
		float balance = .25f;
	};
}
