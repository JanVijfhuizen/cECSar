#pragma once
#include "EntityInfo.h"

namespace game
{
	struct Leg final
	{
		cecsar::EntityInfo root{};
		cecsar::EntityInfo mirror{};

		float moveThreshold = 16;
		float breakDistance = 64;
		float stoppingDistance = 2;
		float speed = 400;

		bool moving = false;
	};
}
