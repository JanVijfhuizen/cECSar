#pragma once
#include <vector>
#include "EntityInfo.h"

namespace game
{
	enum class ColliderType
	{
		Circle, Rectange
	};

	struct Collider final
	{
		ColliderType type{};

		bool isCollider = false;
		bool isStatic = false;

		union
		{
			struct Circle
			{
				float radius;
			} circle;

			struct Rectange
			{
				float width, height;
			} rectangle;
		};

		std::vector<cecsar::EntityInfo> hits;
	};
}
