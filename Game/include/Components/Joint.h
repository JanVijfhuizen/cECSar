#pragma once
#include "EntityInfo.h"

namespace game
{
	struct Joint final
	{
		cecsar::EntityInfo other{};
		float maxDistance = 16;
	};
}
