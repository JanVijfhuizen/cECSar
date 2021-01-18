#pragma once
#include "EntityInfo.h"

namespace game
{
	struct Hand final
	{
		enum class Type
		{
			Left, Right, Both
		};

		Type type = Type::Both;
		cecsar::EntityInfo equipped{};
	};
}
