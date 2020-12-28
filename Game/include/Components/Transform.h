#pragma once
#include "Utils/Vector3.h"
#include "EntityInfo.h"

namespace game
{
	struct Transform final
	{
		utils::Vector3 position;
		float rotation = 0;
		cecsar::EntityInfo parent;
	};
}
