#pragma once
#include "EntityInfo.h"

namespace game
{
	struct Leg final
	{
		cecsar::EntityInfo root{};
		cecsar::EntityInfo mirror{};

		float moveThreshold = 32;
		float breakDistance = 64;
		float stoppingDistance = 4;
		float speed = 400;

		bool moving = false;
	};
}
