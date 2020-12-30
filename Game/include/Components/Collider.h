#pragma once
#include <vector>
#include "EntityInfo.h"

namespace game
{
	enum class ColliderType
	{
		Circle, Rectangle
	};

	struct Collider final
	{
		ColliderType type{};

		bool isTrigger = false;
		bool isStatic = false;

		union
		{
			struct Circle
			{
				float radius;
			} circle;

			struct Rectangle
			{
				float width, height;
			} rectangle;
		};

		std::vector<cecsar::EntityInfo> hits;
	};
}
