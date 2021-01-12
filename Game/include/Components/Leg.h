#pragma once
#include "EntityInfo.h"
#include "Utils/Vector3.h"

namespace game
{
	struct Leg final
	{
		cecsar::EntityInfo root{};
		cecsar::EntityInfo mirror{};

		float moveThreshold = 12;
		float breakDistance = 24;
		float stoppingDistance = 4;
		float speed = 100;

		bool moving = false;
		utils::Vector3 target{};
	};
}
