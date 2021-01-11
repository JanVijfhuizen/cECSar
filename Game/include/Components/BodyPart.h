#pragma once
#include "Utils/Vector3.h"

namespace game
{
	struct BodyPart final
	{
		cecsar::EntityInfo root{};
		utils::Vector3 rootPosition{};
	};
}
